/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "LinkLayerRouter.h"

#include <sstream>
#include <assert.h>


#include <openpal/LoggableMacros.h>
#include <openpal/IPhysicalLayerAsync.h>

#include "ILinkContext.h"
#include "LinkFrame.h"

using namespace std;
using namespace openpal;

namespace opendnp3
{

LinkLayerRouter::LinkLayerRouter(	const Logger& logger,
                                    IPhysicalLayerAsync* apPhys,
                                    openpal::TimeDuration minOpenRetry,
                                    openpal::TimeDuration maxOpenRetry,
									openpal::IEventHandler<ChannelState>* pStateHandler_,
									openpal::IShutdownHandler* pShutdownHandler_,
                                    IOpenDelayStrategy* pStrategy) :
	Loggable(logger),
	PhysicalLayerMonitor(logger, apPhys, minOpenRetry, maxOpenRetry, pStrategy),
	pStateHandler(pStateHandler_),
	pShutdownHandler(pShutdownHandler_),
	mReceiver(logger, this),
	mTransmitting(false)
{}

bool LinkLayerRouter::IsRouteInUse(const LinkRoute& route)
{
	auto pNode = records.FindFirst([&](const Record& record){ return record.route == route; });
	return (pNode != nullptr);
}

bool LinkLayerRouter::AddContext(ILinkContext* pContext, const LinkRoute& route)
{
	assert(pContext != nullptr);

	if (IsRouteInUse(route))
	{
		return false;
	}
	else
	{
		auto pNode = records.FindFirst([&](const Record& record){ return record.pContext == pContext; });
		if (pNode)
		{
			LOG_BLOCK(LogLevel::Error, "Context cannot be bound 2x");
			return false;
		}
		else
		{
			// record is always disabled by default
			Record(pContext, route);
			return records.Add(Record(pContext, route));
		}		
	}	
}

bool LinkLayerRouter::Enable(ILinkContext* pContext)
{	
	auto pNode = records.FindFirst([&](const Record& rec){ return rec.pContext == pContext;  });

	if(pNode)	
	{
		if(!(pNode->value.enabled))
		{
			pNode->value.enabled = true;
			if (this->IsLowerLayerUp())
			{
				pNode->value.pContext->OnLowerLayerUp();
			}
			this->Start(); // idempotent call to start router
		}
		return true; // already enabled
	}
	else
	{
		return false;
	}
}

bool LinkLayerRouter::Disable(ILinkContext* pContext)
{
	auto pNode = records.FindFirst([&](const Record& rec){ return rec.pContext == pContext;  });

	if (pNode)
	{
		if (pNode->value.enabled)
		{
			pNode->value.enabled = false;
			if (this->IsLowerLayerUp())
			{
				pNode->value.pContext->OnLowerLayerDown();
			}

			if (!this->HasEnabledContext())
			{
				this->Suspend();
			}
		}
		return true;		
	}
	else
	{
		return false;
	}
}

bool LinkLayerRouter::Remove(ILinkContext* pContext)
{
	auto pNode = records.RemoveFirst([&](const Record& rec){ return rec.pContext == pContext; });

	if(pNode)
	{		
		if (this->GetState() == ChannelState::OPEN && pNode->value.enabled)
		{
			pNode->value.pContext->OnLowerLayerDown();			
		}

		// if no contexts are enabled, suspend the router
		if (!HasEnabledContext())
		{
			this->Suspend();
		}

		return true;
	}
	else
	{
		return false;		
	}
}

ILinkContext* LinkLayerRouter::GetEnabledContext(const LinkRoute& route)
{
	auto pNode = records.FindFirst(
		[&](const Record& rec)
		{ 
			return rec.enabled && (rec.route == route);
		}
	);

	return pNode ? pNode->value.pContext : nullptr;	
}


ILinkContext* LinkLayerRouter::GetDestination(uint16_t aDest, uint16_t aSrc)
{
	LinkRoute route(aSrc, aDest);

	ILinkContext* pDest = GetEnabledContext(route);

	if(pDest == nullptr)
	{

		ERROR_BLOCK(LogLevel::Warning, "Frame w/ unknown route: " << route.ToString(), DLERR_UNKNOWN_ROUTE);
	}

	return pDest;
}

//////////////////////////////////////////////////////
// IFrameSink Implementation
//////////////////////////////////////////////////////

void LinkLayerRouter::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->Ack(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->Nack(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->LinkStatus(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->NotSupported(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->TestLinkStatus(aIsMaster, aFcb, aDest, aSrc);
}
void LinkLayerRouter::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->ResetLinkStates(aIsMaster, aDest, aSrc);
}
void LinkLayerRouter::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->RequestLinkStatus(aIsMaster, aDest, aSrc);
}
void LinkLayerRouter::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->ConfirmedUserData(aIsMaster, aFcb, aDest, aSrc, arBuffer);
}
void LinkLayerRouter::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->UnconfirmedUserData(aIsMaster, aDest, aSrc, arBuffer);
}

void LinkLayerRouter::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	// The order is important here. You must let the receiver process the byte or another read could write
	// over the buffer before it is processed
	mReceiver.OnRead(arBuffer.Size()); //this may trigger callbacks to the local ILinkContext interface
	if(mpPhys->CanRead())   // this is required because the call above could trigger the layer to be closed
	{
		auto buff = mReceiver.WriteBuff();
		mpPhys->AsyncRead(buff); //start another read
	}
}

bool LinkLayerRouter::Transmit(const LinkFrame& arFrame)
{
	LinkRoute lr(arFrame.GetDest(), arFrame.GetSrc());

	if (this->GetEnabledContext(lr))
	{
		if(this->IsLowerLayerUp())
		{
			this->mTransmitQueue.push_back(arFrame);
			this->CheckForSend();
			return true;
		}
		else
		{
			ERROR_BLOCK(LogLevel::Error, "Cannot queue a frame while router if offline", DLERR_ROUTER_OFFLINE);
			return false;
		}
	}
	else
	{
		ERROR_BLOCK(LogLevel::Error, "Ignoring unassociated transmit w/ route: " << lr.ToString(), DLERR_UNKNOWN_ROUTE);
		return false;
	}
}

void LinkLayerRouter::OnStateChange(ChannelState state)
{
	if (this->pStateHandler)
	{
		pStateHandler->OnEvent(state);		
	}
}

void LinkLayerRouter::OnShutdown()
{
	if (pShutdownHandler)
	{
		pShutdownHandler->OnShutdown();
	}
}

bool LinkLayerRouter::HasEnabledContext()
{
	auto pNode = records.FindFirst([](const Record& rec){ return rec.enabled; });
	return (pNode != nullptr);
}

void LinkLayerRouter::_OnSendSuccess()
{
	assert(mTransmitQueue.size() > 0);
	assert(mTransmitting);
	/* TODO - remove dead code
	const LinkFrame& f = mTransmitQueue.front();
	LinkRoute lr(f.GetDest(), f.GetSrc());
	ILinkContext* pContext = this->GetContext(lr);
	assert(pContext != nullptr);
	*/
	mTransmitting = false;
	mTransmitQueue.pop_front();
	this->CheckForSend();
}

void LinkLayerRouter::_OnSendFailure()
{
	LOG_BLOCK(LogLevel::Error, "Unexpected _OnSendFailure");
	mTransmitting = false;
	this->CheckForSend();
}

void LinkLayerRouter::CheckForSend()
{
	if(mTransmitQueue.size() > 0 && !mTransmitting && mpPhys->CanWrite())
	{
		mTransmitting = true;
		const LinkFrame& f = mTransmitQueue.front();
		LOG_BLOCK(LogLevel::Interpret, "~> " << f.ToString());
		ReadOnlyBuffer buff(f.GetBuffer(), f.GetSize());
		mpPhys->AsyncWrite(buff);
	}
}

void LinkLayerRouter::OnPhysicalLayerOpenSuccessCallback()
{
	if(mpPhys->CanRead())
	{
		auto buff = mReceiver.WriteBuff();
		mpPhys->AsyncRead(buff);
	}

	records.Foreach(
		[](const Record& rec)
		{
			if (rec.enabled)
			{
				rec.pContext->OnLowerLayerUp();
			}
		}
	);
	
}

void LinkLayerRouter::OnPhysicalLayerCloseCallback()
{
	// reset the state of receiver
	mReceiver.Reset();

	// Drop frames queued for transmit and tell the contexts that the router has closed
	mTransmitting = false;
	mTransmitQueue.erase(mTransmitQueue.begin(), mTransmitQueue.end());
	
	records.Foreach(
		[](const Record& rec)
		{
			if (rec.enabled)
			{
				rec.pContext->OnLowerLayerDown();
			}
		}
	);
}

}



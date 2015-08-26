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
#include "PriLinkLayerStates.h"


#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/link/LinkLayer.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

PriStateBase& PriStateBase::OnAck(LinkLayer& link, bool rxBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnNack(LinkLayer& link, bool rxBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnLinkStatus(LinkLayer& link, bool rxBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnNotSupported(LinkLayer& link, bool rxBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnTransmitResult(LinkLayer& link, bool success)
{
	FORMAT_LOG_BLOCK(link.ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());	
	return *this;
}

PriStateBase& PriStateBase::OnTimeout(LinkLayer& link)
{
	FORMAT_LOG_BLOCK(link.ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());
	return *this;
}

PriStateBase& PriStateBase::TrySendConfirmed(LinkLayer& link, ITransportSegment&)
{	
	return *this;
}

PriStateBase& PriStateBase::TrySendUnconfirmed(LinkLayer& link, ITransportSegment&)
{	
	return *this;
}

PriStateBase& PriStateBase::TrySendRequestLinkStatus(LinkLayer&)
{
	return *this;
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_Idle PLLS_Idle::instance;

PriStateBase& PLLS_Idle::TrySendUnconfirmed(LinkLayer& link, ITransportSegment& segments)
{	
	auto first = segments.GetSegment();
	auto output = link.ctx.FormatPrimaryBufferWithUnconfirmed(first);	
	link.ctx.QueueTransmit(output, true, link);
	return PLLS_SendUnconfirmedTransmitWait::Instance();	
}

PriStateBase& PLLS_Idle::TrySendConfirmed(LinkLayer& link, ITransportSegment& segments)
{
	if (link.ctx.isRemoteReset)
	{
		link.ctx.ResetRetry();
		auto buffer = link.ctx.FormatPrimaryBufferWithConfirmed(segments.GetSegment(), link.ctx.nextWriteFCB);
		link.ctx.QueueTransmit(buffer, true, link);
		return PLLS_ConfUserDataTransmitWait::Instance();
	}
	else
	{
		link.ctx.ResetRetry();
		link.ctx.QueueResetLinks(link);
		return PLLS_LinkResetTransmitWait::Instance();
	}	
}

PriStateBase& PLLS_Idle::TrySendRequestLinkStatus(LinkLayer& link)
{
	link.ctx.QueueRequestLinkStatus(link);
	return PLLS_RequestLinkStatusTransmitWait::Instance();
}

////////////////////////////////////////////////////////
//	Class SendUnconfirmedTransmitWait
////////////////////////////////////////////////////////

PLLS_SendUnconfirmedTransmitWait PLLS_SendUnconfirmedTransmitWait::instance;

PriStateBase& PLLS_SendUnconfirmedTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (link.ctx.pSegments->Advance())
	{
		auto output = link.ctx.FormatPrimaryBufferWithUnconfirmed(link.ctx.pSegments->GetSegment());
		link.ctx.QueueTransmit(output, true, link);
		return *this;
	}
	else // we're done
	{
		link.ctx.CompleteSendOperation(success);
		return PLLS_Idle::Instance();
	}
}


/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::instance;

PriStateBase& PLLS_LinkResetTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting for an ACK
		link.StartTimer();
		return PLLS_ResetLinkWait::Instance();
	}
	else
	{		
		link.ctx.CompleteSendOperation(success);
		return PLLS_Idle::Instance();
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::instance;

PriStateBase& PLLS_ConfUserDataTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting on an ACK
		link.StartTimer();
		return PLLS_ConfDataWait::Instance();
	}
	else
	{		
		link.ctx.CompleteSendOperation(false);
		return PLLS_Idle::Instance();
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the request link status
/////////////////////////////////////////////////////////////////////////////

PLLS_RequestLinkStatusTransmitWait PLLS_RequestLinkStatusTransmitWait::instance;

PriStateBase& PLLS_RequestLinkStatusTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting on a LINK_STATUS
		link.StartTimer();
		return PLLS_RequestLinkStatusWait::Instance();
	}
	else
	{
		link.FailKeepAlive(false);
		return PLLS_Idle::Instance();
	}
}


////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::instance;

PriStateBase& PLLS_ResetLinkWait::OnAck(LinkLayer& link, bool rxBuffFull)
{
	link.ctx.isRemoteReset = true;
	link.ctx.ResetWriteFCB();
	link.CancelTimer();
	auto buffer = link.ctx.FormatPrimaryBufferWithConfirmed(link.ctx.pSegments->GetSegment(), link.ctx.nextWriteFCB);
	link.ctx.QueueTransmit(buffer, true, link);	
	link.ctx.PostStatusCallback(opendnp3::LinkStatus::RESET);
	return PLLS_ConfUserDataTransmitWait::Instance();
}

PriStateBase& PLLS_ResetLinkWait::OnTimeout(LinkLayer& link)
{
	if(link.ctx.Retry())
	{
		FORMAT_LOG_BLOCK(link.ctx.logger, flags::WARN, "Link reset timeout, retrying %i remaining", link.ctx.numRetryRemaining);
		link.ctx.QueueResetLinks(link);
		return PLLS_LinkResetTransmitWait::Instance();
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Link reset final timeout, no retries remain");		
		link.ctx.CompleteSendOperation(false);
		return PLLS_Idle::Instance();
	}
}

PriStateBase& PLLS_ResetLinkWait::Failure(LinkLayer& link)
{
	link.CancelTimer();	
	link.ctx.CompleteSendOperation(false);
	return PLLS_Idle::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::instance;

PriStateBase& PLLS_ConfDataWait::OnAck(LinkLayer& link, bool rxBuffFull)
{
	link.ctx.ToggleWriteFCB();
	link.CancelTimer();

	if (link.ctx.pSegments->Advance())
	{
		auto buffer = link.ctx.FormatPrimaryBufferWithConfirmed(link.ctx.pSegments->GetSegment(), link.ctx.nextWriteFCB);
		link.ctx.QueueTransmit(buffer, true, link);
		return PLLS_ConfUserDataTransmitWait::Instance();
	}
	else //we're done!
	{		
		link.ctx.CompleteSendOperation(true);
		return PLLS_Idle::Instance();
	}
}

PriStateBase& PLLS_ConfDataWait::OnNack(LinkLayer& link, bool rxBuffFull)
{
	link.ctx.PostStatusCallback(opendnp3::LinkStatus::UNRESET);

	if (rxBuffFull)
	{
		return Failure(link);
	}
	else
	{
		link.ctx.ResetRetry();
		link.CancelTimer();		
		link.ctx.QueueResetLinks(link);
		return PLLS_LinkResetTransmitWait::Instance();
	}
	
}

PriStateBase& PLLS_ConfDataWait::Failure(LinkLayer& link)
{
	link.CancelTimer();	
	link.ctx.CompleteSendOperation(false);
	return PLLS_Idle::Instance();
}

PriStateBase& PLLS_ConfDataWait::OnTimeout(LinkLayer& link)
{
	if (link.ctx.Retry())
	{
		FORMAT_LOG_BLOCK(link.ctx.logger, flags::WARN, "confirmed data timeout, retrying %u remaining", link.ctx.numRetryRemaining);
		auto buffer = link.ctx.FormatPrimaryBufferWithConfirmed(link.ctx.pSegments->GetSegment(), link.ctx.nextWriteFCB);
		link.ctx.QueueTransmit(buffer, true, link);
		return PLLS_ConfUserDataTransmitWait::Instance();		
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Confirmed data final timeout, no retries remain");
		link.ctx.PostStatusCallback(opendnp3::LinkStatus::UNRESET);
		link.ctx.CompleteSendOperation(false);
		return PLLS_Idle::Instance();
	}
}

////////////////////////////////////////////////////////
//	Class PLLS_RequestLinkStatusWait
////////////////////////////////////////////////////////

PLLS_RequestLinkStatusWait PLLS_RequestLinkStatusWait::instance;

PriStateBase& PLLS_RequestLinkStatusWait::OnNack(LinkLayer& link, bool)
{
	link.CancelTimer();
	link.FailKeepAlive(false);
	return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnLinkStatus(LinkLayer& link, bool)
{
	link.CancelTimer();
	link.CompleteKeepAlive();
	return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnNotSupported(LinkLayer& link, bool)
{
	link.CancelTimer();
	link.FailKeepAlive(false);
	return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnTimeout(LinkLayer& link)
{
	SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Link status request - response timeout");
	link.FailKeepAlive(true);
	return PLLS_Idle::Instance();
}

} //end namepsace


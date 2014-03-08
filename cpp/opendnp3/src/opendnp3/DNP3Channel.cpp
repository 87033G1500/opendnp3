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
#include "DNP3Channel.h"

#include <openpal/IPhysicalLayerAsync.h>

#include "opendnp3/master/MasterStackImpl.h"
#include "opendnp3/outstation/OutstationStackImpl.h"
#include "ExecutorPause.h"

using namespace openpal;

namespace opendnp3
{

DNP3Channel::DNP3Channel(
							openpal::Logger logger,
							openpal::TimeDuration minOpenRetry,
							openpal::TimeDuration maxOpenRetry,
							IOpenDelayStrategy* pStrategy,
							IPhysicalLayerAsync* pPhys,
							std::function<void(DNP3Channel*)> onShutdown
						) :
	Loggable(logger),
	mpPhys(pPhys),
	mOnShutdown(onShutdown),
	mRouter(logger.GetSubLogger("Router"), mpPhys.get(), minOpenRetry, maxOpenRetry, pStrategy),
	mGroup(pPhys->GetExecutor())
{

}

DNP3Channel::~DNP3Channel()
{
	this->Cleanup();
}

void DNP3Channel::Shutdown()
{
	this->Cleanup();
	mOnShutdown(this);
}

void DNP3Channel::AddStateListener(std::function<void (ChannelState)> aListener)
{
	mRouter.AddStateListener(aListener);
}

openpal::IExecutor* DNP3Channel::GetExecutor()
{
	return mpPhys->GetExecutor();
}

void DNP3Channel::Cleanup()
{
	std::set<IStack*> copy(mStacks);
	for(auto pStack: copy) pStack->Shutdown();
	{
		ExecutorPause p(mpPhys->GetExecutor());
		this->mGroup.Shutdown();	// no more task callbacks
		this->mRouter.Shutdown();	// start shutting down the router
	}
	mRouter.WaitForShutdown();
}

IMaster* DNP3Channel::AddMaster(const std::string& arLoggerId, LogLevel aLevel, ISOEHandler* apPublisher, IUTCTimeSource* apTimeSource, const MasterStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	ExecutorPause p(mpPhys->GetExecutor());
	if(mRouter.IsRouteInUse(route)) 
	{
		LOG_BLOCK(LogLevel::Error, "Route already in use: " << route.ToString());
		return nullptr;
	}
	else {
		auto logger = mLogger.GetSubLogger(arLoggerId, aLevel);		
		auto routeFunc = GetEnableDisableRoute(mpPhys->GetExecutor(), &mRouter, route);
		auto shutdownFunc = [this, route](IStack * apStack) { this->OnStackShutdown(apStack, route); };
		auto pMaster = new MasterStackImpl(logger, mpPhys->GetExecutor(), apPublisher, apTimeSource, &mGroup, arCfg, routeFunc, shutdownFunc);
		pMaster->SetLinkRouter(&mRouter);
		mStacks.insert(pMaster);
		mRouter.AddContext(pMaster->GetLinkContext(), route);		
		return pMaster;
	}
}

IOutstation* DNP3Channel::AddOutstation(const std::string& arLoggerId, LogLevel aLevel, ICommandHandler* apCmdHandler, ITimeWriteHandler* apTimeWriteHandler, const SlaveStackConfig& arCfg)
{
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	ExecutorPause p(mpPhys->GetExecutor());
	if(mRouter.IsRouteInUse(route)) 
	{
		LOG_BLOCK(LogLevel::Error, "Route already in use: " << route.ToString());
		return nullptr;
	}
	else 
	{
		auto logger = mLogger.GetSubLogger(arLoggerId, aLevel);
		auto routeFunc = GetEnableDisableRoute(mpPhys->GetExecutor(), &mRouter, route);
		auto shutdownFunc = [this, route](IStack * apStack) { this->OnStackShutdown(apStack, route); };
		auto pOutstation = new OutstationStackImpl(logger, mpPhys->GetExecutor(), apTimeWriteHandler, apCmdHandler, arCfg, routeFunc, shutdownFunc);
		pOutstation->SetLinkRouter(&mRouter);
		mStacks.insert(pOutstation);
		mRouter.AddContext(pOutstation->GetLinkContext(), route);		
		return pOutstation;
	}
}

std::function<void (bool)> DNP3Channel::GetEnableDisableRoute(IExecutor* apExecutor,  LinkLayerRouter* apRouter, LinkRoute aRoute)
{
	return [apExecutor, apRouter, aRoute](bool enable) {
		apExecutor->Post([=](){
			if(enable) apRouter->EnableRoute(aRoute);
			else apRouter->DisableRoute(aRoute);
		});
	};
}

void DNP3Channel::OnStackShutdown(IStack* apStack, LinkRoute route)
{
	mStacks.erase(apStack);
	{
		ExecutorPause p(mpPhys->GetExecutor());
		mRouter.RemoveContext(route);
	}
	delete apStack;
}

}

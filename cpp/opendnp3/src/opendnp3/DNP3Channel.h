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
#ifndef __DNP3_CHANNEL_H_
#define __DNP3_CHANNEL_H_

#include <openpal/IShutdownHandler.h>
#include <openpal/LogRoot.h>
#include <openpal/LogConfig.h>
#include <openpal/StaticLinkedList.h>

#include "opendnp3/IChannel.h"
#include "opendnp3/outstation/OutstationStackConfig.h"
#include "opendnp3/master/AsyncTaskGroup.h"
#include "opendnp3/link/LinkLayerRouter.h"

#include <memory>

namespace openpal
{
class PhysicalLayerAsyncBase;
}


namespace opendnp3
{

class IStack;
class DNP3Stack;
class IOutstation;
class ICommandHandler;
class ITimeWriteHandler;

class DNP3Channel: public IChannel, private openpal::IShutdownHandler, private openpal::ITypedShutdownHandler<DNP3Stack*>
{
	enum class State
	{
	    READY,
	    SHUTTING_DOWN,
	    SHUTDOWN
	};

public:
	DNP3Channel(
		char const* id,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    IOpenDelayStrategy* pStrategy,
	    openpal::PhysicalLayerAsyncBase* pPhys,
	    openpal::ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler_,
	    openpal::IEventHandler<ChannelState>* pStateHandler_
	);

	// public interface, callable only from outside
	void BeginShutdown() override final;

	openpal::IExecutor* GetExecutor();

	virtual openpal::LogFilters GetLogFilters() const override final;

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final;

	IMaster* AddMaster(		char const* id,
	                        ISOEHandler* pPublisher,
	                        openpal::IUTCTimeSource* pTimeSource,
	                        const MasterStackConfig& cfg);

	IOutstation* AddOutstation( char const* id,
	                            ICommandHandler* pCmdHandler,
	                            ITimeWriteHandler* pTimeWriteHandler,
	                            const OutstationStackConfig& cfg);

	// Helper functions only available inside DNP3Manager

private:

	void InitiateShutdown();

	// shutdown call for the router
	void OnShutdown() override final;

	// shutdown from the stack
	void OnShutdown(DNP3Stack* apStack) override final;

	void CheckForFinalShutdown();

	std::auto_ptr<PhysicalLayerAsyncBase> pPhys;

	Logger logger;

	State state;
	openpal::ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler;

	LinkLayerRouter router;
	AsyncTaskGroup group;

	openpal::StaticLinkedList<DNP3Stack*, uint16_t, sizes::MAX_STACKS_PER_CHANNEL> stacks;

};

}

#endif


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
#ifndef __CHANNEL_SET_H_
#define __CHANNEL_SET_H_

#include <set>

#include <cstdint>
#include <memory>

#include <mutex>
#include <condition_variable>

#include <openpal/Logger.h>
#include <openpal/TimeDuration.h>
#include <openpal/IMutex.h>
#include <openpal/IShutdownHandler.h>
#include <openpal/IEventHandler.h>

#include "opendnp3/gen/ChannelState.h"
#include "opendnp3/link/IOpenDelayStrategy.h"

namespace asiopal
{
	class PhysicalLayerBase;
}

namespace asiodnp3
{

class IChannel;
class DNP3Channel;

class ChannelSet : private openpal::ITypedShutdownHandler<DNP3Channel*>
{

public:
	
	~ChannelSet();

	IChannel* CreateChannel(	openpal::LogRoot* pRoot,
	                            openpal::TimeDuration minOpenRetry,
	                            openpal::TimeDuration maxOpenRetry,
								asiopal::PhysicalLayerBase* pPhys,	                            
								opendnp3::IOpenDelayStrategy* pOpenStrategy = opendnp3::ExponentialBackoffStrategy::Inst());

	/// Synchronously shutdown all channels. Block until complete.
	void Shutdown();

private:

	std::mutex mutex;
	std::condition_variable condition;

	std::set<DNP3Channel*> channels;

	void OnShutdown(DNP3Channel* apChannel) override final;
};

}

#endif

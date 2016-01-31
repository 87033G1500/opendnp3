/*
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

#include "asiopal/StrandExecutor.h"

#include "asiopal/SteadyClock.h"

#include <chrono>

using namespace openpal;

namespace asiopal
{
	
StrandExecutor::StrandExecutor(asio::io_service& service) : strand(service)
{

}

std::shared_ptr<StrandExecutor> StrandExecutor::Create(asio::io_service& service)
{
	return std::shared_ptr<StrandExecutor>(new StrandExecutor(service));
}

MonotonicTimestamp GetTime()
{
	auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(asiopal_steady_clock::now().time_since_epoch()).count();
	return openpal::MonotonicTimestamp(millisec);
}

ITimer* StrandExecutor::Start(const TimeDuration&, const Action0& runnable)
{
	return nullptr;
}

ITimer* StrandExecutor::Start(const MonotonicTimestamp&, const Action0& runnable)
{
	return nullptr;
}

void StrandExecutor::Post(const Action0& runnable)
{
	auto self(shared_from_this());
	auto callback = [self, runnable]() 
	{
		runnable.Apply();
	};
	strand.post(callback);
}

}

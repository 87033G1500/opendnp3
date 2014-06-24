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
#include "ASIOExecutor.h"

#include "TimerASIO.h"

#include <asio.hpp>
#include <functional>

using namespace std;

namespace asiopal
{

ASIOExecutor::ASIOExecutor(asio::io_service& service) : strand(service)
{

}

ASIOExecutor::~ASIOExecutor()
{
	for(auto pTimer : allTimers) delete pTimer;
}

openpal::MonotonicTimestamp ASIOExecutor::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

openpal::ITimer* ASIOExecutor::Start(const openpal::TimeDuration& delay, const openpal::Runnable& runnable)
{	
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_from_now(std::chrono::milliseconds(delay.GetMilliseconds()));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

openpal::ITimer* ASIOExecutor::Start(const openpal::MonotonicTimestamp& time, const openpal::Runnable& runnable)
{	
	TimerASIO* pTimer = GetTimer();
	pTimer->timer.expires_at(std::chrono::steady_clock::time_point(std::chrono::milliseconds(time.milliseconds)));
	this->StartTimer(pTimer, runnable);
	return pTimer;
}

void ASIOExecutor::Post(const openpal::Runnable& runnable)
{
	auto captured = [runnable]() { runnable.Apply(); };
	strand.post(captured);
}

TimerASIO* ASIOExecutor::GetTimer()
{
	TimerASIO* pTimer;
	if(idleTimers.size() == 0)
	{
		pTimer = new TimerASIO(strand);
		allTimers.push_back(pTimer);
	}
	else
	{
		pTimer = idleTimers.front();
		idleTimers.pop_front();
	}

	pTimer->canceled = false;
	return pTimer;
}

void ASIOExecutor::StartTimer(TimerASIO* pTimer, const openpal::Runnable& runnable)
{	
	auto callback = [runnable, this, pTimer](const std::error_code& ec){ this->OnTimerCallback(ec, pTimer, runnable); };
	pTimer->timer.async_wait(strand.wrap(callback));
}

void ASIOExecutor::OnTimerCallback(const std::error_code& ec, TimerASIO* pTimer, const openpal::Runnable& runnable)
{	
	idleTimers.push_back(pTimer);
	if (!(ec || pTimer->canceled))
	{
		runnable.Apply();
	}
}

} //end namespace


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
#ifndef __ASIO_EXECUTOR_H_
#define __ASIO_EXECUTOR_H_

#include <openpal/executor/IExecutor.h>

#include "Synchronized.h"

#include <asio.hpp>
#include <queue>

namespace asiopal
{

class TimerASIO;

class ASIOExecutor : public openpal::IExecutor
{
	
public:

	ASIOExecutor(asio::io_service& service);
	~ASIOExecutor();

	virtual openpal::MonotonicTimestamp GetTime() override final;
	virtual openpal::ITimer* Start(const openpal::TimeDuration&, const openpal::Runnable& runnable)  override final;
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const openpal::Runnable& runnable)  override final;
	virtual void Post(const openpal::Runnable& runnable) override final;

	// Gracefully wait for all timers to finish
	void WaitForShutdown();

	// access to the underlying strand is provided for wrapping callbacks
	asio::strand strand;

private:

	void InitiateShutdown(Synchronized<bool>& handler);

	void CheckForShutdown();

	Synchronized<bool>* pShutdownSignal;

	TimerASIO* GetTimer();

	void StartTimer(TimerASIO*, const openpal::Runnable& runnable);	

	typedef std::deque<TimerASIO*> TimerQueue;

	TimerQueue allTimers;
	TimerQueue idleTimers;	

	void OnTimerCallback(const std::error_code&, TimerASIO*, const openpal::Runnable& runnable);
};

}

#endif


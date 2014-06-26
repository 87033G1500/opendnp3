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

#ifndef __MASTER_SCHEDULER_H_
#define __MASTER_SCHEDULER_H_

#include <openpal/executor/Action1.h>
#include <openpal/executor/IExecutor.h>
#include <openpal/container/Settable.h>
#include <openpal/container/StaticLinkedList.h>

#include "opendnp3/master/MasterTasks.h"
#include "opendnp3/master/PollTask.h"
#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/IMasterScheduler.h"
#include "opendnp3/master/TaskRecord.h"

#include "opendnp3/Configure.h"

namespace opendnp3
{

class MasterScheduler : public IMasterScheduler
{		

public:

	typedef openpal::Action1<ICommandProcessor&> CommandErasure;

	MasterScheduler(	openpal::Logger* pLogger,
						MasterTasks& tasks,
						openpal::IExecutor& executor,			
						IScheduleCallback& callback
					);

	// ---------- Implement IMasterScheduler ----------- 
	
	virtual void Schedule(IMasterTask& task, const openpal::TimeDuration& delay) override final;

	virtual void SetBlocking(IMasterTask& task, const openpal::TimeDuration& delay) override final;
	
	virtual bool Demand(IMasterTask& task) override final;

	// ---------- other public functions ----------------

	/**
	* @return Task to start or nullptr if no tasks are available
	*/
	IMasterTask* Start(const MasterParams& params);

	/*
	* Startup
	*/
	void OnLowerLayerUp(const MasterParams& params);

	/**
	* Cleanup all existing tasks & cancel any timers
	*/
	void OnLowerLayerDown();	

	/*
	* Schedule a command to run
	*/
	void ScheduleCommand(const CommandErasure& action);	

	/**
	* Add a new poll to the scheduler
	*/
	PollTask* AddPollTask(const PollTask& pt);

	/*
	* Called when the master observes the IIN::DeviceRestart bit
	*/
	void OnRestartDetected(const MasterParams& params);

	/*
	* Called when the master observes the IIN::NeedTime bit
	*/
	void OnNeedTimeDetected(const MasterParams& params);

private:

	static IMasterTask* NextEnabledTask(IMasterTask* pCurrent, const MasterParams& params, MasterTasks& tasks);

	static IMasterTask* CurrentOrNextEnabledTask(IMasterTask* pCurrent, const MasterParams& params, MasterTasks& tasks);

	IMasterTask* FindTaskToStart(const MasterParams& params);	

	IMasterTask* GetStartupTask(const MasterParams& params);

	IMasterTask* GetTimeSyncTask(const MasterParams& params);

	IMasterTask* GetPeriodicTask(const MasterParams& params, const openpal::MonotonicTimestamp& now);

	openpal::ListNode<TaskRecord>* GetEarliestExpirationTime();	

	void ReportFailure(const CommandErasure& action, CommandResult result);	
	
	void StartOrRestartTimer(const openpal::MonotonicTimestamp& expiration);

	void StartTimer(const openpal::TimeDuration& timeout);

	void StartTimer(const openpal::MonotonicTimestamp& expiration);

	void OnTimerExpiration();	

	bool CancelScheduleTimer();	

	bool IsTaskActive(IMasterTask* pTask);

	// ----------- static configuration ---------

	openpal::IExecutor* pExecutor;
	IScheduleCallback* pCallback;
	MasterTasks* pStaticTasks;

	// ----------- dynamic state -----------

	bool isOnline;
	bool outstationNeedsTime;

	openpal::ITimer* pTimer;
	IMasterTask* pCurrentTask;
	IMasterTask* pStartupTask;

	openpal::Settable<TaskRecord> blockingTask;

	openpal::StaticLinkedList<TaskRecord, uint16_t, sizes::MAX_MASTER_POLL_TASKS> periodicTasks;

	openpal::StaticLinkedList<PollTask, uint16_t, sizes::MAX_MASTER_POLL_TASKS> pollTasks;
	
};

}



#endif

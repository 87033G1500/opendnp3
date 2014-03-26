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
#ifndef __MASTER_H_
#define __MASTER_H_

#include "opendnp3/StaticSizeConfiguration.h"

#include "opendnp3/app/IAppUser.h"
#include "opendnp3/app/IAppLayer.h"

#include "opendnp3/master/MasterConfig.h"
#include "opendnp3/master/ISOEHandler.h"
#include "opendnp3/master/MasterScan.h"
#include "opendnp3/master/MasterSchedule.h"
#include "opendnp3/master/QueuedCommandProcessor.h"

#include "opendnp3/master/StartupTasks.h"
#include "opendnp3/master/DataPoll.h"
#include "opendnp3/master/CommandTask.h"

#include <openpal/IExecutor.h>
#include <openpal/IUTCTimeSource.h>
#include <openpal/StaticBuffer.h>

namespace openpal
{

class IExecutor;

}

namespace opendnp3
{

class ITask;
class AsyncTaskGroup;
class ITimeSource;
class AsyncTaskContinuous;
class AsyncTaskBase;
class CopyableBuffer;
class AMS_Base;

/**
 * Represents a DNP3 Master endpoint. The tasks functions can perform all the
 * various things that a Master might need to do.
 *
 * Coordination of tasks is handled by a higher level task scheduler.
 */
class Master : public IAppUser, private ICommandProcessor
{
	friend class AMS_Base;
	friend class AMS_Idle;
	friend class AMS_OpenBase;
	friend class AMS_Waiting;
	friend class MasterSchedule;

public:

	Master(openpal::Logger, MasterConfig aCfg, IAppLayer*, ISOEHandler*, AsyncTaskGroup*, openpal::IExecutor*, openpal::IUTCTimeSource* apTimeSrc);
	virtual ~Master() {}

	ICommandProcessor* GetCommandProcessor()
	{
		return &mCommandQueue;
	}

	MasterScan GetIntegrityScan();

	MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate);

	/* Implement IAppUser - callbacks from the app layer */

	void OnLowerLayerUp();
	void OnLowerLayerDown();

	void OnSolSendSuccess();
	void OnSolFailure();

	void OnUnsolSendSuccess();
	void OnUnsolFailure();

	// override the response functions
	void OnPartialResponse(const APDUResponseRecord&);
	void OnFinalResponse(const APDUResponseRecord&);
	void OnUnsolResponse(const APDUResponseRecord&);

	// These methods are inherited privately
	void SelectAndOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback);

private:	

	/* Task functions used for scheduling */

	void WriteIIN(ITask* apTask);
	void IntegrityPoll(ITask* apTask);
	void EventPoll(ITask* apTask, int aClassMask);
	void ChangeUnsol(ITask* apTask, bool aEnable, int aClassMask);
	void SyncTime(ITask* apTask);
	void ProcessCommand(ITask* apTask);

	openpal::StaticBuffer<sizes::MAX_APDU_BUFFER_SIZE> requestBuffer;

	IINField mLastIIN;						// last IIN received from the outstation

	void ProcessIIN(const IINField& arIIN);	// Analyze IIN bits and react accordingly
	void ProcessDataResponse(const APDUResponseRecord& aRecord);	// Read data output of solicited or unsolicited response and publish
	void StartTask(MasterTaskBase*, bool aInit);	// Starts a task running	

	openpal::IExecutor* pExecutor;
	IAppLayer* mpAppLayer;					// lower application layer
	ISOEHandler* mpSOEHandler;
	AsyncTaskGroup* mpTaskGroup;			// How task execution is controlled
	openpal::IUTCTimeSource* mpTimeSrc;		// Access to UTC, normally system time but can be a mock for testing

	AMS_Base* mpState;						// Pointer to active state, start in TLS_Closed
	MasterTaskBase* mpTask;					// The current master task
	ITask* mpScheduledTask;					// The current scheduled task	
	
	/* --- Task plumbing --- */

	MasterSchedule mSchedule;				// The machinery needed for scheduling

	ClassPoll mClassPoll;					// used to perform integrity/exception scans
	ClearRestartIIN mClearRestart;			// used to clear the restart
	ConfigureUnsol mConfigureUnsol;			// manipulates how the outstation does unsolictied reporting
	TimeSync mTimeSync;						// performs time sync on the outstation
	CommandTask mCommandTask;				// performs command execution


	QueuedCommandProcessor mCommandQueue;	// Threadsafe queue for buffering command requests
};

}



#endif

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

#include "IMasterState.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/master/MasterState.h"

namespace opendnp3
{

// --------- Default Actions -------------

IMasterState* IMasterState::OnStart(MasterState& mstate)
{
	return this;
}

IMasterState* IMasterState::OnResponse(MasterState& mstate, const APDUResponseHeader& response, const openpal::ReadBufferView& objects)
{
	FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Not expecting a response, sequence: %u", response.control.SEQ);
	return this;
}

IMasterState* IMasterState::OnResponseTimeout(MasterState& mstate)
{
	SIMPLE_LOG_BLOCK(mstate.logger, flags::ERR, "Unexpected response timeout");
	return this;
}

// --------- IDLE -------------

MasterStateIdle MasterStateIdle::instance;

IMasterState* MasterStateIdle::OnStart(MasterState& mstate)
{
	if(mstate.isSending)
	{
		return this;
	}
	else
	{
		auto task = mstate.scheduler.Start();
		
		if (task.IsDefined())
		{				
			if (mstate.BeginNewTask(task))
			{
				return &MasterStateWaitForResponse::Instance();
			}
			else
			{
				return &MasterStateTaskReady::Instance();
			}
		}
		else
		{
			return this;
		}
	}
}

// --------- READY -------------

MasterStateTaskReady MasterStateTaskReady::instance;

IMasterState* MasterStateTaskReady::OnStart(MasterState& mstate)
{
	if (mstate.isSending)
	{
		return this;
	}
	else
	{
		if (mstate.ResumeActiveTask())
		{			
			return &MasterStateWaitForResponse::Instance();
		}
		else
		{
			return this;
		}
	}	
}

// --------- WaitForResponse -------------

MasterStateWaitForResponse MasterStateWaitForResponse::instance;

IMasterState* MasterStateWaitForResponse::OnResponse(MasterState& mstate, const APDUResponseHeader& response, const openpal::ReadBufferView& objects)
{
	if (response.control.SEQ != mstate.solSeq)
	{
		FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Response with bad sequence: %u", response.control.SEQ);
		return this;
	}


	mstate.responseTimer.Cancel();

	mstate.solSeq.Increment();

	auto now = mstate.pExecutor->GetTime();
		
	auto result = mstate.pActiveTask->OnResponse(response, objects, now);

	if (response.control.CON)
	{
		mstate.QueueConfirm(APDUHeader::SolicitedConfirm(response.control.SEQ));
	}

	switch (result)
	{
		case(IMasterTask::ResponseResult::OK_CONTINUE) :
			mstate.StartResponseTimer();			
			return this;
		case(IMasterTask::ResponseResult::OK_REPEAT) :
			return MasterStateTaskReady::Instance().OnStart(mstate);
		default:
			// task completed or failed, either way go back to idle			
			mstate.CompleteActiveTask();
			return &MasterStateIdle::Instance();
	}	
}

IMasterState* MasterStateWaitForResponse::OnResponseTimeout(MasterState& mstate)
{			
	auto now = mstate.pExecutor->GetTime();
	mstate.pActiveTask->OnResponseTimeout(now);	
	mstate.solSeq.Increment();
	mstate.CompleteActiveTask();
	return &MasterStateIdle::Instance();
}

}


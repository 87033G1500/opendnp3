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

#include "MasterState.h"

#include "opendnp3/app/APDULogging.h"

#include <assert.h>

namespace opendnp3
{
	MasterState::MasterState(
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		ILowerLayer& lower,
		ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		IScheduleCallback& scheduleCallback,
		const MasterParams& params_,
		ITaskLock& taskLock
		) :

		logger(root.GetLogger()),
		pExecutor(&executor),
		pLower(&lower),
		params(params_),
		pSOEHandler(&SOEHandler),
		pTaskLock(&taskLock),
		pApplication(&application),
		pScheduleCallback(&scheduleCallback),
		isOnline(false),
		isSending(false),		
		responseTimer(executor),
		tasks(params, logger, application, SOEHandler, application),
		scheduler(executor, scheduleCallback),
		txBuffer(params.maxTxFragSize),
		pState(&MasterStateIdle::Instance())
	{}

	void MasterState::OnStart()
	{
		if (isOnline)
		{
			this->pState = pState->OnStart(*this);
		}
	}

	void MasterState::OnResponseTimeout()
	{
		if (isOnline)
		{
			this->pState = pState->OnResponseTimeout(*this);
		}
	}

	void MasterState::ReleaseActiveTask()
	{
		if (this->pActiveTask.IsDefined())
		{
			if (this->pActiveTask->IsRecurring())
			{
				this->scheduler.Schedule(std::move(this->pActiveTask));
			}
			else
			{
				this->pActiveTask.Release();
			}
		}
	}

	void MasterState::OnResponse(const APDUResponseHeader& response, const openpal::ReadBufferView& objects)
	{
		if (isOnline)
		{
			this->pState = pState->OnResponse(*this, response, objects);
		}
	}

	void MasterState::QueueConfirm(const APDUHeader& header)
	{
		this->confirmQueue.push_back(header);
		this->CheckConfirmTransmit();
	}

	bool MasterState::CheckConfirmTransmit()
	{
		if (this->isSending || this->confirmQueue.empty())
		{
			return false;
		}

		auto confirm = this->confirmQueue.front();
		APDUWrapper wrapper(this->txBuffer.GetWriteBufferView());
		wrapper.SetFunction(confirm.function);
		wrapper.SetControl(confirm.control);
		this->Transmit(wrapper.ToReadOnly());	
		this->confirmQueue.pop_front();
		return true;
	}

	void MasterState::Transmit(const openpal::ReadBufferView& data)
	{
		logging::ParseAndLogRequestTx(this->logger, data);
		assert(!this->isSending);
		this->isSending = true;
		this->pLower->BeginTransmit(data);
	}

	void MasterState::StartResponseTimer()
	{
		auto timeout = [this](){ this->OnResponseTimeout(); };
		this->responseTimer.Start(this->params.responseTimeout, timeout);
	}

	void MasterState::PostCheckForTask()
	{
		auto callback = [this]() { this->OnStart(); };
		this->pExecutor->PostLambda(callback);
	}

	bool MasterState::GoOffline()
	{
		if (isOnline)
		{
			auto now = pExecutor->GetTime();

			if (pActiveTask.IsDefined())
			{
				pActiveTask->OnLowerLayerClose(now);
				pActiveTask.Release();
			}

			pState = &MasterStateIdle::Instance();

			pTaskLock->OnLayerDown();

			scheduler.OnLowerLayerDown();

			responseTimer.Cancel();

			solSeq = unsolSeq = 0;
			isOnline = isSending = false;

			return true;			
		}
		else
		{
			return false;
		}		
	}

	bool MasterState::GoOnline()
	{
		if (isOnline)
		{
			return false;
		}
		else
		{
			isOnline = true;
			pTaskLock->OnLayerUp();
			tasks.Initialize(scheduler);
			scheduler.OnLowerLayerUp();
			return true;
		}
	}
	void MasterState::StartTask(IMasterTask& task)
	{
		APDURequest request(this->txBuffer.GetWriteBufferView());
		task.BuildRequest(request, this->solSeq);
		this->StartResponseTimer();		
		this->Transmit(request.ToReadOnly());
	}

}



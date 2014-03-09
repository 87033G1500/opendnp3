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
#include "AsyncTaskGroup.h"

#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include "AsyncTaskContinuous.h"

#include <openpal/IExecutor.h>

#include <openpal/Location.h>

#include <algorithm>
#include <assert.h>

using namespace openpal;

namespace opendnp3
{

AsyncTaskGroup::AsyncTaskGroup(IExecutor* apExecutor) :
	mIsRunning(false),
	mShutdown(false),
	mpExecutor(apExecutor),
	mpTimer(nullptr)
{

}

AsyncTaskGroup::~AsyncTaskGroup()
{
	this->Shutdown();

	for(AsyncTaskBase * p : mTaskVec) delete p;
}

AsyncTaskBase* AsyncTaskGroup::Add(openpal::TimeDuration aPeriod, openpal::TimeDuration aRetryDelay, int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskBase* pTask;
	if(aPeriod.GetMilliseconds() < 0)
		pTask = new AsyncTaskNonPeriodic(aRetryDelay, aPriority, arCallback, this, arName);
	else
		pTask = new AsyncTaskPeriodic(aPeriod, aRetryDelay, aPriority, arCallback, this, arName);

	mTaskVec.push_back(pTask);
	return pTask;
}

void AsyncTaskGroup::ResetTasks(int aMask)
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		if(!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
	}
}

AsyncTaskContinuous* AsyncTaskGroup::AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskContinuous* pTask = new AsyncTaskContinuous(aPriority, arCallback, this, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

bool AsyncTaskGroup::Remove(AsyncTaskBase* apTask)
{
	for(TaskVec::iterator i = mTaskVec.begin(); i != mTaskVec.end(); ++i)
	{
		if(*i == apTask)
		{
			delete *i;
			mTaskVec.erase(i);
			return true;
		}
	}
	return false;
}

void AsyncTaskGroup::Shutdown()
{
	if(mpTimer)
	{
		mpTimer->Cancel();
		mpTimer = nullptr;
	}

	mShutdown = true;
}

void AsyncTaskGroup::Enable()
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable()
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		p->SilentDisable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Enable(int aMask)
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		if((p->GetFlags() & aMask) != 0) p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable(int aMask)
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		if((p->GetFlags() & aMask) != 0) p->SilentDisable();
	}
	this->CheckState();
}

AsyncTaskBase* AsyncTaskGroup::GetNext(const MonotonicTimestamp& arTime)
{
	this->Update(arTime);
	TaskVec::iterator max = max_element(mTaskVec.begin(), mTaskVec.end(), AsyncTaskBase::LessThanGroupLevel);

	AsyncTaskBase* pRet = nullptr;
	if(max != mTaskVec.end())
	{
		AsyncTaskBase* p = *max;
		if(!p->IsRunning() && p->IsEnabled()) pRet = p;
	}

	return pRet;
}

void AsyncTaskGroup::CheckState()
{
	if(!mShutdown)
	{
		auto now = mpExecutor->GetTime();
		AsyncTaskBase* pTask = GetNext(now);

		if(pTask == nullptr) return;
		if(pTask->NextRunTime().milliseconds == MonotonicTimestamp::Max().milliseconds) return;

		if(pTask->NextRunTime().milliseconds <= now.milliseconds)
		{
			mIsRunning = true;
			pTask->Dispatch();
		}
		else
		{
			this->RestartTimer(pTask->NextRunTime());
		}
	}
}

void AsyncTaskGroup::OnCompletion()
{
	assert(mIsRunning);
	mIsRunning = false;
	this->CheckState();
}


openpal::MonotonicTimestamp AsyncTaskGroup::GetCurrentTime() const
{
	return mpExecutor->GetTime();
}

void AsyncTaskGroup::Update(const MonotonicTimestamp& arTime)
{
	for(AsyncTaskBase * p : mTaskVec)
	{
		p->UpdateTime(arTime);
	}
}


void AsyncTaskGroup::RestartTimer(const openpal::MonotonicTimestamp& arTime)
{
	if(mpTimer != nullptr)
	{
		if(mpTimer->ExpiresAt().milliseconds != arTime.milliseconds)
		{
			mpTimer->Cancel();
			mpTimer = nullptr;
		}
	}

	if(mpTimer == nullptr)
		mpTimer = mpExecutor->Start(arTime, std::bind(&AsyncTaskGroup::OnTimerExpiration, this));
}

void AsyncTaskGroup::OnTimerExpiration()
{
	mpTimer = nullptr;
	this->CheckState();
}

} // end ns

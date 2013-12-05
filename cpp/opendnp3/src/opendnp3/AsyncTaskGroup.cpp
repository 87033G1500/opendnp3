#include "AsyncTaskGroup.h"

#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include "AsyncTaskContinuous.h"

#include <openpal/IExecutor.h>
#include <openpal/Exception.h>
#include <openpal/Location.h>

#include <algorithm>

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

for(AsyncTaskBase * p: mTaskVec) delete p;
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
for(AsyncTaskBase * p: mTaskVec) {
		if(!p->IsRunning() && (p->GetFlags() & aMask)) p->Reset();
	}
}

AsyncTaskContinuous* AsyncTaskGroup::AddContinuous(int aPriority, const TaskHandler& arCallback, const std::string& arName)
{
	AsyncTaskContinuous* pTask = new AsyncTaskContinuous(aPriority, arCallback, this, arName);
	mTaskVec.push_back(pTask);
	return pTask;
}

void AsyncTaskGroup::Remove(AsyncTaskBase* apTask)
{
	for(TaskVec::iterator i = mTaskVec.begin(); i != mTaskVec.end(); ++i) {
		if(*i == apTask) {
			delete *i;
			mTaskVec.erase(i);
			return;
		}
	}
	MACRO_THROW_EXCEPTION(ArgumentException, "Task not found");
}

void AsyncTaskGroup::Shutdown()
{
	if(mpTimer) {
		mpTimer->Cancel();
		mpTimer = nullptr;
	}

	mShutdown = true;
}

void AsyncTaskGroup::Enable()
{
for(AsyncTaskBase * p: mTaskVec) {
		p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable()
{
for(AsyncTaskBase * p: mTaskVec) {
		p->SilentDisable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Enable(int aMask)
{
	for(AsyncTaskBase * p: mTaskVec) {
		if((p->GetFlags() & aMask) != 0) p->SilentEnable();
	}
	this->CheckState();
}

void AsyncTaskGroup::Disable(int aMask)
{
for(AsyncTaskBase * p: mTaskVec) {
		if((p->GetFlags() & aMask) != 0) p->SilentDisable();
	}
	this->CheckState();
}

AsyncTaskBase* AsyncTaskGroup::GetNext(const MonotonicTimestamp& arTime)
{
	this->Update(arTime);
	TaskVec::iterator max = max_element(mTaskVec.begin(), mTaskVec.end(), AsyncTaskBase::LessThanGroupLevel);

	AsyncTaskBase* pRet = nullptr;
	if(max != mTaskVec.end()) {
		AsyncTaskBase* p = *max;
		if(!p->IsRunning() && p->IsEnabled()) pRet = p;
	}

	return pRet;
}

void AsyncTaskGroup::CheckState()
{
	if(!mShutdown) {
		auto now = mpExecutor->GetTime();
		AsyncTaskBase* pTask = GetNext(now);

		if(pTask == nullptr) return;
		if(pTask->NextRunTime().milliseconds == MonotonicTimestamp::Max().milliseconds) return;

		if(pTask->NextRunTime().milliseconds <= now.milliseconds) {
			mIsRunning = true;
			pTask->Dispatch();
		}
		else {
			this->RestartTimer(pTask->NextRunTime());
		}
	}
}

void AsyncTaskGroup::OnCompletion()
{
	if(!mIsRunning) MACRO_THROW_EXCEPTION(InvalidStateException, "Not running");
	mIsRunning = false;
	this->CheckState();
}


openpal::MonotonicTimestamp AsyncTaskGroup::GetCurrentTime() const
{
	return mpExecutor->GetTime();
}

void AsyncTaskGroup::Update(const MonotonicTimestamp& arTime)
{
for(AsyncTaskBase * p: mTaskVec) {
		p->UpdateTime(arTime);
	}
}


void AsyncTaskGroup::RestartTimer(const openpal::MonotonicTimestamp& arTime)
{
	if(mpTimer != nullptr) {
		if(mpTimer->ExpiresAt().milliseconds != arTime.milliseconds) {
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

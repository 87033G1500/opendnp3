#ifndef __MOCK_EXECUTOR_H_
#define __MOCK_EXECUTOR_H_

#include <openpal/IExecutor.h>

#include <map>
#include <queue>
#include <limits>
#include <stddef.h>

namespace opendnp3
{

class MockTimer;

/** @section desc Test class that doles out MockTimer* */
class MockExecutor : public openpal::IExecutor
{
	friend class MockTimer;

public:
	MockExecutor();
	~MockExecutor();

	// Implement IExecutor
	openpal::ITimer* Start(const openpal::MonotonicTimestamp&, const std::function<void ()>&);
	openpal::ITimer* Start(const openpal::TimeDuration&, const std::function<void ()>&);
	void Post(const std::function<void ()>&);
	void PostSync(const std::function<void ()>&);
	openpal::MonotonicTimestamp GetTime();

	/** Turns the auto-post feature on/off. When Auto post is on, Post() is executed synchronously */
	void SetAutoPost(bool aAutoPost) {
		this->mPostIsSynchronous = aAutoPost;
	}

	/**	Call the next (by expiration time) without caring about the time requirement.
		@returns true if a timer was dispatched */
	bool DispatchOne();

	/** Calls DispatchOne() up to some maximum number of times continuing while
		there are still events to dispatch

		@return the number of events dispatched
	*/
	size_t Dispatch(size_t aMaximum = std::numeric_limits<size_t>::max());

	/** @returns The number of active, pending timers and post operations */
	size_t NumActive() {
		return mTimerMap.size() + mPostQueue.size();
	}

	openpal::MonotonicTimestamp NextTimerExpiration();

	void AdvanceTime(openpal::TimeDuration aDuration);


private:

	void Cancel(openpal::ITimer* apTimer);

	typedef std::deque<std::function<void ()>> PostQueue;
	typedef std::multimap<openpal::MonotonicTimestamp, MockTimer*> TimerMap;
	typedef std::deque<MockTimer*> TimerQueue;
	typedef std::deque<openpal::MonotonicTimestamp> TimerExpirationQueue;

	bool mPostIsSynchronous;
	bool mAutoPost;
	openpal::MonotonicTimestamp mCurrentTime;
	PostQueue mPostQueue;
	TimerMap mTimerMap;
	TimerQueue mIdle;
	TimerQueue mAllTimers;
	TimerExpirationQueue mTimerExpirationQueue;

};

/** @section desc Test timer class used in conjunction with MockExecutor */
class MockTimer : public openpal::ITimer
{
	friend class MockExecutor;

public:
	MockTimer(MockExecutor*, const openpal::MonotonicTimestamp&, const std::function<void ()>&);

	//implement ITimer
	void Cancel();
	openpal::MonotonicTimestamp ExpiresAt();

private:
	openpal::MonotonicTimestamp mTime;
	MockExecutor* mpSource;
	std::function<void ()> mCallback;
};

}

#endif


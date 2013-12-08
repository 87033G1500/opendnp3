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
#ifndef __EVENT_BUFFER_BASE_H_
#define __EVENT_BUFFER_BASE_H_

#include "ClassCounter.h"
#include "EventTypes.h"



namespace opendnp3
{

template <class T>
class EventAcceptor
{
public:
	
	virtual void Update(const Event<T>& aEvent) = 0;	
};

/**
 * Base class for the EventBuffer classes (with templating and virtual
 * function for Update to alter event storage behavior)
 *
 * Single-threaded for asynchronous/event-based model.
*/
template <class T, class SetType>
class EventBufferBase : public EventAcceptor<T>
{
public:
	EventBufferBase(size_t aMaxEvents);
	virtual ~EventBufferBase() {}

	void Update(const T& aValue, PointClass aClass, uint32_t aIndex)
	{
		Event<T> e(aValue, aIndex, aClass);
		this->Update(e);
	}

	/**
	 * Adds an event to the buffer, tracking insertion order. Calls _Update
	 * which can be overriden to do special types of insertion.
	 *
	 * @param aEvent Event update to add to the buffer
	 */
	void Update(const Event<T>& aEvent);

	/**
	 * Returns true if the buffer contains any data matching the given
	 * PointClass.
	 *
	 * @param aClass		the class of data to match
	 *
	 * @return				'true' if the buffer contains matching data,
	 * 						'false' if not
	 */
	bool HasClassData(PointClass aClass) {
		return mCounter.GetNum(aClass) > 0;
	}

	/**
	 * Selects data in the buffer that matches the given PointClass, up to
	 * the defined number of entries.
	 *
	 * @param aClass		the class of data to match
	 * @param aMaxEvent		maximum number of events to select
	 *
	 * @return				the number of events selected
	 */
	size_t Select(PointClass aClass, size_t aMaxEvent = std::numeric_limits<size_t>::max());

	/**
	 * Transfers any unwritten events back into the event set.
	 *
	 * @return				the number of events returned to the event set
	 */
	size_t Deselect();


	/**
	 * Remove events that have been written (flagged with 'mWritten=true')
	 * from the selection buffer.
	 *
	 * @return				the number of events removed
	 */
	size_t ClearWrittenEvents();

	/**
	 * Returns an EvtItr iterator object for accessing the data previously
	 * selected through EventBufferBase::Select().
	 *
	 * @return the iterator object to the data from Select()
	 */
	typename EventIterator<T>::Type Begin();

	/**
	 * Returns the number of events that were previously selected through
	 * EventBufferBase::Select().
	 *
	 * @return				the number of events selected
	 */
	size_t NumSelected() {
		return mSelectedEvents.size();
	}

	/**
	 * Returns the number of events that were omitted from the selection
	 * during the last call of EventBufferBase::Select().
	 *
	 * @return				the number of events not selected
	 */
	size_t NumUnselected() {
		return mEventSet.size();
	}

	/**
	 * Returns the total number of events (selected and unselected) being
	 * managed by the EventBufferBase.
	 *
	 * @return				the number of events
	 */
	size_t Size() {
		return mSelectedEvents.size() + mEventSet.size();
	}

	/**
	 * @return the number of events before the buffer will overflow
	 *
	 */
	size_t NumAvailable() {
		return M_MAX_EVENTS - this->Size();
	}

	/**
	 * Returns a flag to indicate whether the buffer has been overflown.
	 *
	 * @return				true if too much data has been written to the
	 * 						buffer, false if not.
	 */
	bool IsOverflown();

	/**
	 * Returns a flag to indicate whether the buffer is full.  A subsequent
	 * write the buffer at this point would lead to IsOverflown() returning
	 * true.
	 *
	 * @return				true if the buffer is full, false if the buffer
	 * 						still has space
	 */
	bool IsFull() {
		return NumUnselected() >= M_MAX_EVENTS;
	}

protected:

	/**
	 * Adds an event to the buffer, tracking insertion order. Calls _Update
	 * which can be overriden to do special types of insertion.
	 *
	 * @param arEvent		Event update to add to the buffer
	 * @param aNewValue		a new sequence value will be assigned to the
	 * 						event if the value is true, otherwise the
	 * 						existing arEvent sequence number will be used
	 */
	void Update(EventInfo<T>& aEvent, bool aNewValue = true);

	/**
	 * Overridable NVII function called by Update.  The default
	 * implementation does a simple insert into mEventSet.
	 *
	 * @param arEvent		Event update to add to the buffer
	 */
	virtual void _Update(const EventInfo<T>& aEvent);

	ClassCounter mCounter;		// counter for class events
	const size_t M_MAX_EVENTS;	// max number of events to accept before setting overflow
	size_t mSequence;			// used to track the insertion order of events into the buffer
	bool mIsOverflown;			// flag that tracks when an overflow occurs

	// vector to hold all selected events until they are cleared or failed back into mEventSet
	typename std::vector< EventInfo<T> > mSelectedEvents;

	// store to keep and order incoming events
	typename SetType::Type mEventSet;
};

template <class T, class SetType>
EventBufferBase <T, SetType> :: EventBufferBase(size_t aMaxEvents) :
	M_MAX_EVENTS(aMaxEvents),
	mSequence(0),
	mIsOverflown(false)
{}

template <class T, class SetType>
void EventBufferBase<T, SetType> :: Update(const Event<T>& aEvent)
{	
	EventInfo<T> info(aEvent);
	this->Update(info, true);

	if(this->NumUnselected() > M_MAX_EVENTS) { //we've overflown and we've got to drop an event
		mIsOverflown = true;
		typename SetType::Type::iterator itr = mEventSet.begin();
		this->mCounter.DecrCount(itr->clazz);
		mEventSet.erase(itr);
	}
}

template <class T, class SetType>
void EventBufferBase<T, SetType> :: Update(EventInfo<T>& aEvent, bool aNewValue)
{
	// prevents numerical overflow of the increasing sequence number
	if(this->Size() == 0) mSequence = 0;

	if(aNewValue) aEvent.mSequence = mSequence++;

	this->_Update(aEvent); // call the overridable NVII function
}

template <class T, class SetType>
void EventBufferBase<T, SetType> :: _Update(const EventInfo<T>& aEvent)
{
	this->mCounter.IncrCount(aEvent.clazz);
	this->mEventSet.insert(aEvent);
}

template <class T, class SetType>
size_t EventBufferBase<T, SetType> :: Deselect()
{
	size_t num = mSelectedEvents.size();

	// put selected events back into the event buffer
	for(size_t i = 0; i < num; i++) this->Update(mSelectedEvents[i], false);

	mSelectedEvents.clear();

	return num;
}

template <class T, class SetType>
bool EventBufferBase <T, SetType> :: IsOverflown()
{
	// if the buffer previously overflowed, but is no longer full, reset the flag
	if(mIsOverflown && this->Size() < M_MAX_EVENTS) mIsOverflown = false;

	return mIsOverflown;
}

template <class T, class SetType>
size_t EventBufferBase<T, SetType> :: ClearWrittenEvents()
{
	auto itr = this->mSelectedEvents.begin();

	size_t num = 0;
	while(itr != this->mSelectedEvents.end() && itr->mWritten) {
		++itr;
		++num;
	}

	this->mSelectedEvents.erase(this->mSelectedEvents.begin(), itr);
	return num;
}

template <class T, class SetType>
typename EventIterator<T>::Type EventBufferBase<T, SetType> :: Begin()
{
	return mSelectedEvents.begin();
}

template <class T, class SetType>
size_t EventBufferBase <T, SetType> :: Select(PointClass aClass, size_t aMaxEvent)
{
	auto i = mEventSet.begin();

	size_t count = 0;

	while( i != mEventSet.end() && count < aMaxEvent) {
		if( ( i->clazz & aClass) != 0 ) {
			mCounter.DecrCount(i->clazz);
			mSelectedEvents.push_back(*i);
			mEventSet.erase(i++);
			++count;
			mSelectedEvents.back().mWritten = false;
		}
		else ++i;
	}

	return count;
}

} //end NS

/* vim: set ts=4 sw=4: */

#endif

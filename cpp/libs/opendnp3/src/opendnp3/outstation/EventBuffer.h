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

#ifndef OPENDNP3_EVENTBUFFER_H
#define OPENDNP3_EVENTBUFFER_H

#include "opendnp3/outstation/IEventReceiver.h"
#include "opendnp3/outstation/IEventSelector.h"
#include "opendnp3/outstation/EventCount.h"
#include "opendnp3/outstation/SelectionWriter.h"
#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/outstation/SOERecord.h"

#include <openpal/container/LinkedList.h>

namespace opendnp3
{

/*
	The event buffer doesn't actually own the buffers, it just creates
    collection facades around the indexable buffers it's given.
	This is done so that the buffers can either be statically or dynamically allocated.

	The sequence of events list in the facade is a doubly linked-list implemented
	in a finite array.  The list is desired for O(1) remove operations from
	arbitrary parts of the list depending on what the user asks for in terms
	of event type or Class1/2/3.

	At worst, selection is O(n) in the SOE length but it has some type/class
	tracking to avoid looping over the SOE list when there are no more events matching
	the selection criteria.
*/

class EventBuffer : public IEventReceiver, public IEventSelector
{
	
public:

	EventBuffer(const EventBufferConfig& config);
	
	// ------- IEventReceiver ------ 

	virtual void Update(const Event<Binary>& evt, EventBinaryVariation var) override final { this->UpdateAny(evt, var); }	
	virtual void Update(const Event<DoubleBitBinary>& evt, EventDoubleBinaryVariation var) override final { this->UpdateAny(evt, var); }		
	virtual void Update(const Event<Analog>& evt, EventAnalogVariation var) override final { this->UpdateAny(evt, var); }	
	virtual void Update(const Event<Counter>& evt, EventCounterVariation var) override final { this->UpdateAny(evt, var); }	
	virtual void Update(const Event<FrozenCounter>&  evt, EventFrozenCounterVariation var) override final { this->UpdateAny(evt, var); }	
	virtual void Update(const Event<BinaryOutputStatus>& evt, EventBinaryOutputStatusVariation var) override final { this->UpdateAny(evt, var); }	
	virtual void Update(const Event<AnalogOutputStatus>& evt, EventAnalogOutputStatusVariation var) override final { this->UpdateAny(evt, var); }

	// ------- IEventSelector ------ 

	virtual void Unselect() {}

	virtual IINField SelectAll(GroupVariation gv) override final { return IINField(); }

	virtual IINField SelectCount(GroupVariation gv, uint16_t count) override final { return IINField(); }

	// ------- Misc -------

	void Reset(); // called when a transmission fails

	void ClearWritten(); // called when a transmission succeeds

	ClassField UnwrittenClassField() const;
	
	bool IsOverflown();

private:

	void RemoveFromCounts(const SOERecord& record);

	bool RemoveOldestEventOfType(EventType type);

	template <class T>
	void UpdateAny(const Event<T>& evt,  typename T::EventVariation var);

	bool IsAnyTypeOverflown() const;
	bool IsTypeOverflown(EventType type) const;	

	bool overflow;

	EventBufferConfig config;
			
	openpal::LinkedList<SOERecord, uint32_t> events;

	// ---- trakcers

	EventCount totalCounts;
	EventCount selectedCounts;
	EventCount writtenCounts;

	bool HasEnoughSpaceToClearOverflow() const;	
};

template <class T>
void EventBuffer::UpdateAny(const Event<T>& evt, typename T::EventVariation var)
{		
	auto maxForType = config.GetMaxEventsForType(T::EventTypeEnum);
	
	if (maxForType > 0)
	{
		auto currentCount = totalCounts.NumOfType(T::EventTypeEnum);
		
		if (currentCount >= maxForType || events.IsFull())
		{
			this->overflow = true;
			RemoveOldestEventOfType(T::EventTypeEnum);			
		}

		events.Add(SOERecord(evt.value, evt.index, evt.clazz, var));
		totalCounts.Increment(evt.clazz, T::EventTypeEnum);
	}	
}

}

#endif


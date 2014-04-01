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

#include "OutstationEventBuffer.h"

namespace opendnp3
{

OutstationEventBuffer::OutstationEventBuffer(const EventBufferFacade& facade_) :
	overflow(false),
	facade(facade_)
{

}

void OutstationEventBuffer::Reset()
{
	while(facade.selectedEvents.IsNotEmpty())
	{
		auto pNode = facade.selectedEvents.Pop();
		pNode->value.selected = false;
	}

	selectedTracker.Clear();
}

bool OutstationEventBuffer::IsOverflown()
{
	if (overflow && HasEnoughSpaceToClearOverflow())
	{
		overflow = false;
	}

	return overflow;
}

void OutstationEventBuffer::Clear()
{
	while(facade.selectedEvents.IsNotEmpty())
	{
		auto pNode = facade.selectedEvents.Pop();
		switch(pNode->value.type)
		{
		case(EventType::Binary):
			facade.binaryEvents.Release(pNode->value.index);
			break;
		case(EventType::DoubleBitBinary) :
			facade.doubleBinaryEvents.Release(pNode->value.index);
			break;
		case(EventType::Analog):
			facade.analogEvents.Release(pNode->value.index);
			break;
		case(EventType::Counter):
			facade.counterEvents.Release(pNode->value.index);
			break;
		case(EventType::FrozenCounter) :
			facade.frozenCounterEvents.Release(pNode->value.index);
			break;
		case(EventType::BinaryOutputStatus) :
			facade.binaryOutputStatusEvents.Release(pNode->value.index);
			break;
		case(EventType::AnalogOutputStatus) :
			facade.analogOutputStatusEvents.Release(pNode->value.index);
			break;
		}
		facade.sequenceOfEvents.Remove(pNode); // O(1) from SOE
	}

	totalTracker = totalTracker.Subtract(selectedTracker);
	selectedTracker.Clear();
}

EventTracker OutstationEventBuffer::TotalEvents() const
{
	return totalTracker;
}

EventTracker OutstationEventBuffer::UnselectedEvents() const
{
	return totalTracker.Subtract(selectedTracker);
}

EventTracker OutstationEventBuffer::SelectedEvents() const
{
	return selectedTracker;
}

void OutstationEventBuffer::Update(const Event<Binary>& aEvent)
{
	InsertEvent(aEvent, EventType::Binary, facade.binaryEvents);
}

void OutstationEventBuffer::Update(const Event<Analog>& aEvent)
{
	InsertEvent(aEvent, EventType::Analog,  facade.analogEvents);
}

void OutstationEventBuffer::Update(const Event<Counter>& aEvent)
{
	InsertEvent(aEvent, EventType::Counter,  facade.counterEvents);
}

void OutstationEventBuffer::Update(const Event<FrozenCounter>& aEvent)
{
	InsertEvent(aEvent, EventType::FrozenCounter,  facade.frozenCounterEvents);
}

void OutstationEventBuffer::Update(const Event<DoubleBitBinary>& aEvent)
{
	InsertEvent(aEvent, EventType::DoubleBitBinary, facade.doubleBinaryEvents);
}

void OutstationEventBuffer::Update(const Event<BinaryOutputStatus>& aEvent)
{
	InsertEvent(aEvent, EventType::BinaryOutputStatus, facade.binaryOutputStatusEvents);
}

void OutstationEventBuffer::Update(const Event<AnalogOutputStatus>& aEvent)
{
	InsertEvent(aEvent, EventType::AnalogOutputStatus, facade.analogOutputStatusEvents);
}

bool OutstationEventBuffer::HasEnoughSpaceToClearOverflow() const
{
	return	HasSpace(facade.analogEvents) &&
	        HasSpace(facade.analogOutputStatusEvents) &&
	        HasSpace(facade.binaryEvents) &&
	        HasSpace(facade.binaryOutputStatusEvents) &&
	        HasSpace(facade.counterEvents) &&
	        HasSpace(facade.doubleBinaryEvents) &&
	        HasSpace(facade.frozenCounterEvents) &&
	        HasSpace(facade.sequenceOfEvents);
}


uint32_t OutstationEventBuffer::NumUnselectedMatching(const SelectionCriteria& criteria) const
{
	uint32_t count = 0;
	auto unselected = this->UnselectedEvents();
	count += unselected.class1.CountOf(criteria.class1);
	count += unselected.class2.CountOf(criteria.class2);
	count += unselected.class3.CountOf(criteria.class3);
	return count;
}

SelectionIterator OutstationEventBuffer::SelectEvents(const SelectionCriteria& criteria)
{
	return SelectionIterator(this, criteria, facade.sequenceOfEvents.Iterate());
}

}

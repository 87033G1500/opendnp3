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

#include "EventResponseContext.h"

#include <openpal/Serialization.h>

#include "EventResponseTypes.h"

using namespace openpal;

namespace opendnp3
{

EventResponseContext::EventResponseContext(OutstationEventBuffer* pBuffer_, const EventResponseConfig& config_) : 
	isComplete(false),
	pBuffer(pBuffer_),
	config(config_)
{

}


bool EventResponseContext::IsComplete() const
{
	return isComplete;
}

IINField EventResponseContext::ReadAll(const GroupVariationRecord& record)
{
	switch(record.enumeration)
	{
		// ----------- class polls --------------

	case(GroupVariation::Group60Var2) :
		criteria.AddToClass1(events::ALL_TYPES);
		return IINField::Empty;
	case(GroupVariation::Group60Var3) :
		criteria.AddToClass2(events::ALL_TYPES);
		return IINField::Empty;
	case(GroupVariation::Group60Var4) :
		criteria.AddToClass3(events::ALL_TYPES);
		return IINField::Empty;

		// ----------- variation 0 --------------

	case(GroupVariation::Group2Var0) :
		criteria.AddToAllClasses(events::BINARY);
		return IINField::Empty;
	case(GroupVariation::Group22Var0) :
		criteria.AddToAllClasses(events::COUNTER);
		return IINField::Empty;
	case(GroupVariation::Group23Var0) :
		criteria.AddToAllClasses(events::FROZEN_COUNTER);
		return IINField::Empty;
	case(GroupVariation::Group32Var0) :
		criteria.AddToAllClasses(events::ANALOG);
		return IINField::Empty;

	default:
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

void EventResponseContext::Reset()
{
	isComplete = false;
	criteria.Clear();
}

bool EventResponseContext::Load(ObjectWriter& writer)
{
	if (isComplete)
	{
		return true;
	}
	else
	{
		if (criteria.HasSelection())
		{			
			auto iterator = pBuffer->SelectEvents(criteria);
			isComplete = WriteEventHeaders(writer, iterator, config);
			return isComplete;
		}
		else
		{
			return true;
		}
	}	
}

bool EventResponseContext::WriteEventHeaders(ObjectWriter& writer, SelectionIterator& iterator, const EventResponseConfig& config)
{
	uint32_t count = 0;

	while (iterator.HasValue())
	{
		auto numWritten = WriteOneHeader(writer, iterator, config);
		count += numWritten;		
		if (numWritten == 0)
		{
			return false;
		}
		
	}
	
	return true;
}

uint32_t EventResponseContext::WriteOneHeader(ObjectWriter& writer, SelectionIterator& iterator, const EventResponseConfig& config)
{
	switch (iterator.GetValue())
	{
		case(EventType::Binary) :
			return WriteFullHeader<Binary>(writer, iterator, EventResponseTypes::Lookup(config.binary));
							
		case(EventType::DoubleBitBinary) :
			return WriteFullHeader<DoubleBitBinary>(writer, iterator, EventResponseTypes::Lookup(config.doubleBinary));
									 
		case(EventType::Counter) :
			return WriteFullHeader<Counter>(writer, iterator, EventResponseTypes::Lookup(config.counter));
							 
		case(EventType::FrozenCounter) :
			return WriteFullHeader<FrozenCounter>(writer, iterator, EventResponseTypes::Lookup(config.frozenCounter));

		case(EventType::Analog) :
			return WriteFullHeader<Analog>(writer, iterator, EventResponseTypes::Lookup(config.analog));

		case(EventType::BinaryOutputStatus) :
			return WriteFullHeader<BinaryOutputStatus>(writer, iterator, EventResponseTypes::Lookup(config.binaryOutputStatus));

		case(EventType::AnalogOutputStatus) :
			return WriteFullHeader<AnalogOutputStatus>(writer, iterator, EventResponseTypes::Lookup(config.analogOutputStatus));

		default:			
			return 0;
	}
}


}

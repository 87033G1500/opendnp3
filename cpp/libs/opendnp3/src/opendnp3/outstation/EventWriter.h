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
#ifndef __EVENT_WRITER_H_
#define __EVENT_WRITER_H_

#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/EventResponseConfig.h"
#include "opendnp3/outstation/SelectionIterator.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/serialization/Serialization.h>

namespace opendnp3
{

/**
 * Builds and tracks the state of multi-fragmented event responses to READ requests,
 * coordinating with the event buffer
 */
class EventWriter : private openpal::PureStatic
{

public:

	static bool WriteEventHeaders(ObjectWriter& writer, SelectionIterator& iterator, const EventResponseConfig& config);

private:
	
	static uint32_t WriteOneHeader(ObjectWriter& writer, SelectionIterator& iterator, const EventResponseConfig& config);

	// return the number of events written in the header
	template <class T>
	static uint32_t WriteFullHeader(ObjectWriter& writer, SelectionIterator& iterator, IDNP3Serializer<T>& serializer);
};

template <class T>
uint32_t EventWriter::WriteFullHeader(ObjectWriter& ow, SelectionIterator& iterator, IDNP3Serializer<T>& serializer)
{
	uint32_t count = 0;

	auto writer = ow.IterateOverCountWithPrefix<openpal::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX, serializer);
	
	Event<T> evt;
	while (iterator.Read(evt))
	{
		if (writer.Write(evt.value, evt.index))
		{
			++count;
			iterator.SelectCurrent();
			iterator.SeekNext();
		}
		else
		{
			return count;
		}
	}
	
	return count;
	
}

}

#endif

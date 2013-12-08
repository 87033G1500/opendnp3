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
#ifndef __EVENT_TYPES_H_
#define __EVENT_TYPES_H_

#include "DNPDatabaseTypes.h"

#include <opendnp3/VTOData.h>



namespace opendnp3
{

/**
 * Structure for holding event data information. Adds a sequence number and a
 * written flag to the data member.
 */
template<typename T>
struct EventInfo : public Event<T> 
{
	EventInfo(const Event<T>& aEvent) :
		Event<T>(aEvent),
		mSequence(0),
		mWritten(false)
	{}	

	EventInfo() : mSequence(0), mWritten(false) {}

	size_t mSequence;	// sequence number used by the event buffers to record insertion order
	bool mWritten;		// true if the event has been written
};

template <typename EventType>
struct EvtItr {
	typedef typename std::vector< EventType >::iterator Type;
};

typedef EvtItr<EventInfo<Binary>>::Type			BinaryEventIter;
typedef EvtItr<EventInfo<Analog>>::Type			AnalogEventIter;
typedef EvtItr<EventInfo<Counter>>::Type		CounterEventIter;


} //end namespace

/* vim: set ts=4 sw=4: */

#endif

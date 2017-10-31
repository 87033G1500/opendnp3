/*
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
#ifndef OPENDNP3_EVENTWRITEHANDLER_H
#define OPENDNP3_EVENTWRITEHANDLER_H

#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/gen/EventBinaryVariation.h"
#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include "opendnp3/gen/EventCounterVariation.h"
#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include "opendnp3/gen/EventAnalogVariation.h"
#include "opendnp3/gen/EventBinaryOutputStatusVariation.h"
#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"

namespace opendnp3
{

template <class T>
class EventWriter
{
public:
	virtual bool Write(const T& meas, uint16_t index) = 0;
};

template <class T>
class EventCollection
{
public:
	virtual uint16_t WriteSome(EventWriter<T>& handler) = 0;
};

class EventWriteHandler
{
public:
	virtual uint16_t Write(EventBinaryVariation variation, const DNPTime& first, EventCollection<Binary>& items) = 0;
	virtual uint16_t Write(EventDoubleBinaryVariation variation, const DNPTime& first, EventCollection<DoubleBitBinary>& items) = 0;
	virtual uint16_t Write(EventCounterVariation variation, const DNPTime& first, EventCollection<Counter>& items) = 0;
	virtual uint16_t Write(EventFrozenCounterVariation variation, const DNPTime& first, EventCollection<FrozenCounter>& items) = 0;
	virtual uint16_t Write(EventAnalogVariation variation, const DNPTime& first, EventCollection<Analog>& items) = 0;
	virtual uint16_t Write(EventBinaryOutputStatusVariation variation, const DNPTime& first, EventCollection<BinaryOutputStatus>& items) = 0;
	virtual uint16_t Write(EventAnalogOutputStatusVariation variation, const DNPTime& first, EventCollection<AnalogOutputStatus>& items) = 0;
};
}

#endif


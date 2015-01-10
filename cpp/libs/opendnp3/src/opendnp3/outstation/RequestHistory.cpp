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

#include "RequestHistory.h"

#include "opendnp3/link/CRC.h"

namespace opendnp3
{

RequestHistory::RequestHistory(uint32_t maxFragSize) : state(State::FIRST), buffer(maxFragSize)
{
	
}

void RequestHistory::Reset()
{
	state = State::FIRST;
}

APDUEquality RequestHistory::RecordLastRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	auto objectsEqual = (state == State::PREVIOUS) ? this->defered.objects.Equals(objects) : false;
	auto functionsEqual = (this->defered.header.function == header.function);
	
	// now swap the values for the new ones
	state = State::PREVIOUS;
	this->defered = DeferredRequest(header, objects.CopyTo(buffer.GetWriteBufferView()), false);	

	// return the correct equality type
	if (functionsEqual)
	{
		return objectsEqual ? APDUEquality::FULL_EQUALITY : APDUEquality::NONE;
	}
	else
	{
		return objectsEqual ? APDUEquality::OBJECT_HEADERS_EQUAL : APDUEquality::NONE;
	}
}

DeferredRequest RequestHistory::PopDeferedRequest()
{
	if (state == State::DEFERED)
	{
		// the defered request now becomes a previous request
		state = State::PREVIOUS;
		return defered;
	}
	else
	{
		return DeferredRequest();
	}	
}

FunctionCode RequestHistory::GetDeferedFunction() const
{
	return (state == State::DEFERED) ? defered.header.function : FunctionCode::UNKNOWN;	
}

void RequestHistory::DeferRequest(const APDUHeader& header, const openpal::ReadBufferView& objects, bool isRepeat)
{
	state = State::DEFERED;	
	this->defered = DeferredRequest(header, objects.CopyTo(buffer.GetWriteBufferView()), isRepeat);
}

}



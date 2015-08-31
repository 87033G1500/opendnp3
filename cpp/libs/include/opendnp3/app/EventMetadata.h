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
#ifndef OPENDNP3_EVENTMETADATA_H
#define OPENDNP3_EVENTMETADATA_H

#include "opendnp3/app/EventType.h"
#include "opendnp3/gen/PointClass.h"

namespace opendnp3
{

// A null object for types that have no metadata
template <class Target>
struct EmptyMetadata
{
	void SetEventValue(const Target& value) {}
};

// Base class for different types of event metadata
template <class Target>
struct EventMetadata
{
	PointClass clazz;
	Target lastEvent;
	typename Target::EventVariation variation;

	void SetEventValue(const Target& value)
	{
		lastEvent = value;
	}

protected:

	EventMetadata() : clazz(PointClass::Class1), lastEvent(), variation(Target::DefaultEventVariation)
	{}
};

template <class Target>
struct SimpleEventMetadata : EventMetadata<Target>
{
	SimpleEventMetadata() : EventMetadata<Target>()
	{}

	bool IsEvent(const Target& newValue) const
	{
		return this->lastEvent.IsEvent(newValue);
	}
};

//Structure for holding metadata information on points that have support deadbanding
template <class Target, class DeadbandType>
struct DeadbandMetadata : EventMetadata<Target>
{
	DeadbandMetadata() : EventMetadata<Target>(), deadband(0)
	{}

	bool IsEvent(const Target& newValue) const
	{
		return this->lastEvent.IsEvent(newValue, deadband);
	}

	DeadbandType deadband;
};


} //end namespace



#endif

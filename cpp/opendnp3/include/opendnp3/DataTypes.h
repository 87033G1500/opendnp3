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
#ifndef __DATA_TYPES_H_
#define __DATA_TYPES_H_

#include "BaseDataTypes.h"
#include "QualityMasks.h"
#include "MeasurementType.h"

namespace opendnp3
{

/**
	The Binary data type is for describing on-off (boolean) type values. Good examples of
	binaries are alarms, mode settings, enabled/disabled flags etc. Think of it as a status
	LED on a piece of equipment.
*/
class Binary : public TypedMeasurement<bool>
{
public:

	Binary() : TypedMeasurement(false, BQ_RESTART) 
	{}

	Binary(bool aValue) : TypedMeasurement(aValue, GetQual(BQ_ONLINE, aValue))
	{}
	
	Binary(bool aValue, uint8_t aQuality) : TypedMeasurement(aValue, GetQual(aQuality, aValue))
	{}

	Binary(bool aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement(aValue, GetQual(aQuality, aValue), aTime) 
	{}
	
	typedef BinaryQuality QualityType;	
	static const MeasurementType MeasEnum = MeasurementType ::BINARY;
	static const int ONLINE = BQ_ONLINE;	

	static Binary From(uint8_t aQuality)
	{
		return Binary((aQuality & BQ_STATE) != 0, aQuality);
	}

	static Binary From(uint8_t aQuality, uint64_t aTime)
	{
		return Binary((aQuality & BQ_STATE) != 0, aQuality, aTime);
	}	

private:
	static uint8_t GetQual(uint8_t q, bool aValue)
	{
		return (aValue) ? (q | BQ_STATE) : (q & (~BQ_STATE));
	}
};

/**
	ControlStatus is used for describing the current state of a control. It is very infrequently
	used and many masters don't provide any mechanisms for reading these values so their use is
	strongly discouraged, a Binary should be used instead.
*/
class ControlStatus : public TypedMeasurement<bool>
{
public:
	ControlStatus() : TypedMeasurement(false, TQ_RESTART) 
	{}

	ControlStatus(bool aValue) : TypedMeasurement(aValue, GetQual(TQ_ONLINE, aValue))
	{}

	ControlStatus(bool aValue, uint8_t aQuality) : TypedMeasurement(aValue, GetQual(aQuality, aValue))
	{}

	ControlStatus(bool aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement(aValue, GetQual(aQuality, aValue), aTime) 
	{}
	
	typedef ControlQuality QualityType;
	static const MeasurementType MeasEnum = MeasurementType::CONTROL_STATUS;
	static const int ONLINE = TQ_ONLINE;	

	static ControlStatus From(uint8_t aQuality)
	{
		return ControlStatus((aQuality & TQ_STATE) != 0, aQuality);
	}

private:
	static uint8_t GetQual(uint8_t q, bool aValue)
	{
		return (aValue) ? (q | TQ_STATE) : (q & (~TQ_STATE));
	}
};

/**
	Analogs are used for variable data points that usuually reflect a real world value.
	Good examples are current, voltage, sensor readouts, etc. Think of a speedometer gauge.
*/

class Analog : public TypedMeasurement<double>
{
public:
	Analog() : TypedMeasurement(AQ_RESTART) 
	{}

	Analog(double aValue) : TypedMeasurement(aValue, AQ_ONLINE) 
	{}

	Analog(double aValue, uint8_t aQuality) : TypedMeasurement(aValue, aQuality) 
	{}

	Analog(double aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<double>(aValue, aQuality, aTime) 
	{}
	
	typedef AnalogQuality QualityType;
	static const MeasurementType MeasEnum = MeasurementType::ANALOG;
	static const int ONLINE = AQ_ONLINE;

	static Analog From(double aValue)
	{
		return Analog(aValue);
	}

	static Analog From(double aValue, uint8_t aQuality)
	{
		return Analog(aValue, aQuality);
	}

	static Analog From(double aValue, uint8_t aQuality, int64_t aTime)
	{
		return Analog(aValue, aQuality, aTime);
	}
};

/**
	Counters are used for describing generally increasing values (non-negative!). Good examples are
	total power consumed, max voltage. Think odometer on a car.
*/
class Counter : public TypedMeasurement<uint32_t>
{
public:

	static Counter From(uint32_t aValue, uint8_t aQuality)
	{
		return Counter(aValue, aQuality);
	}

	Counter() : TypedMeasurement(0, CQ_RESTART) {}

	Counter(uint32_t aValue) : TypedMeasurement<uint32_t>(aValue, CQ_ONLINE) 
	{}

	Counter(uint32_t aValue, uint8_t aQuality) : TypedMeasurement<uint32_t>(aValue, aQuality) 
	{}

	Counter(uint32_t aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<uint32_t>(aValue, aQuality, aTime) 
	{}
	
	typedef CounterQuality QualityType;
	static const int ONLINE = CQ_ONLINE;
	static const MeasurementType MeasEnum = MeasurementType::COUNTER;	

	static Counter From(uint32_t aValue)
	{
		return Counter(aValue);
	}

	static Counter From(uint32_t aValue, uint8_t aQuality, int64_t aTime)
	{
		return Counter(aValue, aQuality, aTime);
	}
};

/**
	Describes the last set value of the setpoint. Like the ControlStatus data type it is not
	well supportted and its generally better practice to use an explict analog.
*/
class SetpointStatus : public TypedMeasurement<double>
{
public:
	
	SetpointStatus() : TypedMeasurement<double>(PQ_RESTART) {}

	SetpointStatus(double aValue) : TypedMeasurement<double>(aValue, PQ_ONLINE) 
	{}
	
	SetpointStatus(double aValue, uint8_t aQuality) : TypedMeasurement<double>(aValue, aQuality) 
	{}

	SetpointStatus(double aValue, uint8_t aQuality, int64_t aTime) : TypedMeasurement<double>(aValue, aQuality, aTime) 
	{}
	
	typedef SetpointQuality QualityType;
	static const int ONLINE = PQ_ONLINE;
	static const MeasurementType MeasEnum = MeasurementType::SETPOINT_STATUS;

	static SetpointStatus From(double aValue, uint8_t aQuality)
	{
		return SetpointStatus(aValue, aQuality);
	}
};

}

#endif

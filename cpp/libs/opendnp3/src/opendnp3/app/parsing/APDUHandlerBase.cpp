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
#include "APDUHandlerBase.h"

using namespace openpal;

namespace opendnp3
{

APDUHandlerBase::APDUHandlerBase(openpal::Logger logger_) :
	logger(logger_),
	numIgnoredHeaders(0),
	errors(),
	cto(0),
	ctoHeader(-1),
	ctoMode(TimestampMode::INVALID),
	currentHeader(0)
{

}

TimestampMode APDUHandlerBase::ModeFromType(GroupVariation gv)
{
	return TypeHasTimestamp(gv) ? TimestampMode::SYNCHRONIZED : TimestampMode::INVALID;
}

void APDUHandlerBase::Reset()
{
	numIgnoredHeaders = 0;
	errors.Clear();
	cto = 0;
	ctoHeader = 0;
	ctoMode = TimestampMode::INVALID;
	currentHeader = 0;
}

IINField APDUHandlerBase::Errors() const
{
	return errors;
}

void APDUHandlerBase::AllObjects(const HeaderRecord& record)
{
	errors |= this->ProcessAllObjects(record);
	++currentHeader;
}

void APDUHandlerBase::OnRangeRequest(const HeaderRecord& record, const Range& range)
{
	errors |= this->ProcessRangeRequest(record, range);
	++currentHeader;
}

void APDUHandlerBase::OnCountRequest(const HeaderRecord& record, uint16_t count)
{
	errors |= this->ProcessCountRequest(record, count);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<IINValue, uint16_t>>& meas)
{
	errors |= this->ProcessIIN(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const IterableBuffer<Group50Var1>& objects)
{
	errors |= this->ProcessCountOf(record, objects);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var1>& times)
{
	Group51Var1 object;
	if (times.ReadOnlyValue(object))
	{
		cto = object.time;
		ctoHeader = currentHeader;
		ctoMode = TimestampMode::SYNCHRONIZED;
	}
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var2>& times)
{
	Group51Var2 object;
	if (times.ReadOnlyValue(object))
	{
		cto = object.time;
		ctoHeader = currentHeader;
		ctoMode = TimestampMode::UNSYNCHRONIZED;
	}
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const IterableBuffer<Group52Var2>& objects)
{
	errors |= this->ProcessCountOf(record, objects);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group2Var3)
	{
		errors |= this->ProcessIndexPrefixCTO(record, meas);
	}
	else
	{
		errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	}

	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group4Var3)
	{
		errors |= this->ProcessIndexPrefixCTO(record, meas);
	}
	else
	{
		errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	}

	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas)
{		
	errors |= this->ProcessIndexPrefix(record, meas);	
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas)
{
	errors |= this->ProcessRange(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas)
{
	errors |= this->ProcessIndexPrefix(record, ModeFromType(record.enumeration), meas);
	++currentHeader;
}

IINField APDUHandlerBase::ProcessUnsupportedHeader()
{
	++numIgnoredHeaders;
	return IINField(IINBit::FUNC_NOT_SUPPORTED);
}

IINField APDUHandlerBase::ProcessAllObjects(const HeaderRecord& record)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRangeRequest(const HeaderRecord& record, const Range& range)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountRequest(const HeaderRecord& record, uint16_t count)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIIN(const HeaderRecord& record, const IterableBuffer<IndexedValue<IINValue, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group50Var1>& objects)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group52Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}


IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas)
{
	return ProcessUnsupportedHeader();
}

TimestampMode APDUHandlerBase::GetCTO(uint64_t& cto_)
{
	if((ctoMode != TimestampMode::INVALID) && (currentHeader == (ctoHeader + 1)))
	{
		cto_ = cto;
		return ctoMode;
	}	
	else
	{
		return TimestampMode::INVALID;
	}
}

}

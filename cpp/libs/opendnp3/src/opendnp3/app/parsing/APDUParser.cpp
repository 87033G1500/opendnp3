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

#include "APDUParser.h"


#include "opendnp3/LogLevels.h"
#include "opendnp3/ErrorCodes.h"

#include "opendnp3/gen/QualifierCode.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/app/MeasurementFactory.h"

#include "opendnp3/app/parsing/ObjectHeaderParser.h"
#include "opendnp3/app/parsing/CountParser.h"
#include "opendnp3/app/parsing/RangeParser.h"
#include "opendnp3/app/parsing/CountIndexParser.h"

#include <openpal/logging/LogMacros.h>


using namespace openpal;

namespace opendnp3
{

ParseResult APDUParser::ParseTwoPass(const openpal::ReadBufferView& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger, ParserSettings settings)
{
	if(pHandler)
	{
		// do two state parsing process with logging but no handling on the first pass
		auto result = ParseSinglePass(buffer, pLogger, nullptr, settings);
		// if the first pass was successful, do a 2nd pass with the handler but no logging
		return (result == ParseResult::OK) ? ParseSinglePass(buffer, nullptr, pHandler, settings) : result;			
	}
	else
	{
		return ParseSinglePass(buffer, pLogger, pHandler, settings);
	}
}

ParseResult APDUParser::ParseSinglePass(const openpal::ReadBufferView& buffer, openpal::Logger* pLogger, IAPDUHandler* pHandler, const ParserSettings& settings)
{
	ReadBufferView copy(buffer);
	while(copy.Size() > 0)
	{
		auto result = ParseHeader(copy, pLogger, settings, pHandler);
		if (result != ParseResult::OK)
		{
			return result;
		}
	}
	return ParseResult::OK;
}

ParseResult APDUParser::ParseHeader(ReadBufferView& buffer, openpal::Logger* pLogger, const ParserSettings& settings, IAPDUHandler* pHandler)
{
	ObjectHeader header;
	auto result = ObjectHeaderParser::ParseObjectHeader(header, buffer, pLogger);
	if (result == ParseResult::OK)
	{
		auto gv = GroupVariationRecord::GetRecord(header.group, header.variation);

		if (gv.enumeration == GroupVariation::UNKNOWN)
		{
			FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_UNKNOWN_GROUP_VAR, "Unknown object %i / %i", gv.group, gv.variation);
			return ParseResult::UNKNOWN_OBJECT;
		}
		else
		{									
			return APDUParser::ParseQualifier(buffer, pLogger, HeaderRecord(gv, header.qualifier), settings, pHandler);
		}
	}	
	else
	{
		return result;
	}
}

ParseResult APDUParser::ParseQualifier(ReadBufferView& buffer, openpal::Logger* pLogger, const HeaderRecord& record, const ParserSettings& settings, IAPDUHandler* pHandler)
{
	switch (record.GetQualifierCode())
	{
		case(QualifierCode::ALL_OBJECTS) :
			return HandleAllObjectsHeader(pLogger, record, settings, pHandler);			 
			 
		case(QualifierCode::UINT8_CNT) :
			return CountParser::ParseHeader(buffer, NumParser::OneByte(), settings, record, pLogger, pHandler);

		case(QualifierCode::UINT16_CNT) :
			return CountParser::ParseHeader(buffer, NumParser::TwoByte(), settings, record, pLogger, pHandler);

		case(QualifierCode::UINT8_START_STOP) :
			return RangeParser::ParseHeader(buffer, NumParser::OneByte(), settings, record, pLogger, pHandler);

		case(QualifierCode::UINT16_START_STOP) :
			return RangeParser::ParseHeader(buffer, NumParser::TwoByte(), settings, record, pLogger, pHandler);

		case(QualifierCode::UINT8_CNT_UINT8_INDEX) :
			return CountIndexParser::ParseHeader(buffer, NumParser::OneByte(), settings, record, pLogger, pHandler);

		case(QualifierCode::UINT16_CNT_UINT16_INDEX) :
			return CountIndexParser::ParseHeader(buffer, NumParser::TwoByte(), settings, record, pLogger, pHandler);

		default:
			FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_UNKNOWN_QUALIFIER, "Unknown qualifier %x", record.qualifier);
			return ParseResult::UNKNOWN_QUALIFIER;
	}
}

ParseResult APDUParser::HandleAllObjectsHeader(openpal::Logger* pLogger, const HeaderRecord& record, const ParserSettings& settings, IAPDUHandler* pHandler)
{
	FORMAT_LOGGER_BLOCK(pLogger, settings.Filters(),
		"%03u,%03u - %s - %s",
		record.group,		
		record.variation,
		GroupVariationToString(record.enumeration),
		QualifierCodeToString(QualifierCode::ALL_OBJECTS));

	if (pHandler)
	{
		pHandler->AllObjects(record);
	}

	return ParseResult::OK;
}

}


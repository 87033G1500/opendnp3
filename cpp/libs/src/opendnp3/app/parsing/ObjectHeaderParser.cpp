/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ObjectHeaderParser.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serialization.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/GroupVariationRecord.h"

using namespace openpal;

namespace opendnp3
{

ObjectHeader::ObjectHeader() : group(0), variation(0), qualifier(0) {}

ParseResult ObjectHeaderParser::ParseObjectHeader(ObjectHeader& header, RSlice& buffer, Logger* pLogger)
{
    if (buffer.Size() < 3)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for header");
        return ParseResult::NOT_ENOUGH_DATA_FOR_HEADER;
    }

    header.group = UInt8::ReadBuffer(buffer);
    header.variation = UInt8::ReadBuffer(buffer);
    header.qualifier = UInt8::ReadBuffer(buffer);
    return ParseResult::OK;
}

bool ObjectHeaderParser::ReadFirstGroupVariation(const openpal::RSlice& objects, GroupVariation& gv)
{
    RSlice copy(objects);
    ObjectHeader oheader;
    if (ObjectHeaderParser::ParseObjectHeader(oheader, copy, nullptr) != ParseResult::OK)
    {
        return false;
    }

    gv = GroupVariationRecord::GetRecord(oheader.group, oheader.variation).enumeration;

    return true;
}

} // namespace opendnp3

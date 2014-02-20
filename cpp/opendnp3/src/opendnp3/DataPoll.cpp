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
#include "DataPoll.h"


#include <openpal/Exception.h>
#include <opendnp3/PointClass.h>
#include <openpal/LoggableMacros.h>

#include "APDUParser.h"
#include  "MeasurementHandler.h"

using namespace openpal;

namespace opendnp3
{

/* DataPoll - base class */

DataPoll::DataPoll(Logger& arLogger, ISOEHandler* pHandler_) :
	MasterTaskBase(arLogger),
	pHandler(pHandler_)
{}

TaskResult DataPoll::_OnPartialResponse(const APDUResponseRecord& record)
{
	this->ReadData(record);
	return TR_CONTINUE;
}

TaskResult DataPoll::_OnFinalResponse(const APDUResponseRecord& record)
{
	this->ReadData(record);
	return TR_SUCCESS;
}

void DataPoll::ReadData(const APDUResponseRecord& record)
{
	MeasurementHandler handler(this->mLogger, this->pHandler);
	auto res = APDUParser::ParseTwoPass(record.objects, &handler);
	if(res != APDUParser::Result::OK)
	{				
		LOG_BLOCK(LogLevel::Warning, "Error parsing response headers: " << static_cast<int>(res)); // TODO - turn these into strings
	}
}

/* Class Poll */

ClassPoll::ClassPoll(Logger& arLogger, ISOEHandler* pHandler_) :
	DataPoll(arLogger, pHandler_),
	mClassMask(PC_INVALID)
{}

void ClassPoll::Set(int aClassMask)
{
	mClassMask = aClassMask;
}

/*
void ClassPoll::ConfigureRequest(APDU& arAPDU)
{
	if (mClassMask == PC_INVALID) {
		MACRO_THROW_EXCEPTION(InvalidStateException, "Class mask has not been set");
	}

	arAPDU.Set(FunctionCode::READ);
	if (mClassMask & PC_CLASS_0) arAPDU.DoPlaceholderWrite(Group60Var1::Inst());
	if (mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if (mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if (mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}
*/


} //end ns



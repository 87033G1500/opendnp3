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

#include "APDUWrapper.h"

#include <openpal/Configure.h>
#include <assert.h>

#include "AppControlField.h"

using namespace openpal;

namespace opendnp3
{

APDUWrapper::APDUWrapper() : valid(false)
{

}

APDUEquality APDUWrapper::Compare(uint32_t headerSize, const ReadBufferView& lhs, const ReadBufferView& rhs)
{
	if (lhs.Size() < headerSize || rhs.Size() < headerSize)
	{
		return APDUEquality::NONE;
	}
	else
	{
		
		auto bodyEqual = lhs.Skip(headerSize).Equals(rhs.Skip(headerSize));
		if (bodyEqual)
		{
			auto headerEqual = lhs.Take(headerSize).Equals(rhs.Take(headerSize));
			return headerEqual ? APDUEquality::FULL_EQUALITY : APDUEquality::OBJECT_HEADERS_EQUAL;
		}
		else
		{
			return APDUEquality::NONE;
		}
	}
}

APDUWrapper::APDUWrapper(const openpal::WriteBufferView& buffer_) : valid(true), buffer(buffer_), remaining(buffer_)
{
	assert(buffer.Size() >= 2); // need a control & function at a minimum
	remaining.Advance(2);
}

bool APDUWrapper::IsValid() const
{
	return valid;
}

HeaderWriter APDUWrapper::GetWriter()
{
	return HeaderWriter(&remaining);
}

uint32_t APDUWrapper::Remaining() const
{
	return remaining.Size();
}

void APDUWrapper::SetFunction(FunctionCode code)
{
	assert(buffer.IsNotEmpty());
	buffer[1] = FunctionCodeToType(code);
}

FunctionCode APDUWrapper::GetFunction() const
{
	assert(buffer.IsNotEmpty());
	return FunctionCodeFromType(buffer[1]);
}

AppControlField APDUWrapper::GetControl() const
{
	assert(buffer.IsNotEmpty());
	return AppControlField(buffer[0]);
}

void APDUWrapper::SetControl(AppControlField control)
{
	buffer[0] = control.ToByte();
}

uint32_t APDUWrapper::Size() const
{
	return buffer.Size() - remaining.Size();
}

openpal::ReadBufferView APDUWrapper::ToReadOnly() const
{	
	return buffer.ToReadOnly().Take(this->Size());
}

}


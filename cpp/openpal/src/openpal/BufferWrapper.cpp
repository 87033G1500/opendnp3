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
#include "BufferWrapper.h"

#include "Configure.h"
#include <cstring>
#include <assert.h>

namespace openpal
{
ReadOnlyBuffer::ReadOnlyBuffer(): HasSize(0), mpBuffer(nullptr)
{}

ReadOnlyBuffer::ReadOnlyBuffer(uint8_t const* apBuffer, uint32_t aSize) :
	HasSize(aSize),
	mpBuffer(apBuffer)
{}

void ReadOnlyBuffer::CopyTo(uint8_t* apDest) const
{
	memcpy(apDest, mpBuffer, size);
}

ReadOnlyBuffer ReadOnlyBuffer::Truncate(uint32_t aSize) const
{
	assert(aSize <= size);
	return ReadOnlyBuffer(mpBuffer, aSize);
}

void ReadOnlyBuffer::ZeroSize()
{
	size = 0;
}

bool ReadOnlyBuffer::Equals(const ReadOnlyBuffer& rhs) const
{
	if (this->Size() == rhs.Size())
	{
		return memcmp(mpBuffer, rhs.mpBuffer, Size()) == 0;
	}
	else
	{
		return false;
	}
}

void ReadOnlyBuffer::Advance(uint32_t aNum)
{
	assert(aNum <= size);
	mpBuffer += aNum;
	size -= aNum;
}

WriteBuffer WriteBuffer::Empty()
{
	return WriteBuffer();
}

WriteBuffer::WriteBuffer(const WriteBuffer& copy) : HasSize(copy) , mpBuffer(copy.mpBuffer)
{

}

WriteBuffer::WriteBuffer(): HasSize(0), mpBuffer(nullptr)
{

}

WriteBuffer::WriteBuffer(uint8_t* apBuffer, uint32_t aSize) :
	HasSize(aSize),
	mpBuffer(apBuffer)
{}

void WriteBuffer::Clear()
{
	mpBuffer = nullptr;
	size = 0;
}

WriteBuffer WriteBuffer::Truncate(uint32_t aNum) const
{
	assert(aNum <= size);
	return WriteBuffer(mpBuffer, aNum);
}

void WriteBuffer::Advance(uint32_t aNum)
{
	assert(aNum <= size);
	mpBuffer += aNum;
	size -= aNum;
}

ReadOnlyBuffer WriteBuffer::ToReadOnly() const
{
	return ReadOnlyBuffer(mpBuffer, size);
}

}



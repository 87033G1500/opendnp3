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
#include "ShiftableBuffer.h"


#include <openpal/Location.h>
#include <assert.h>
#include <memory.h>
#include <cstring>

namespace opendnp3
{

ShiftableBuffer::ShiftableBuffer(uint32_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
}


ShiftableBuffer::ShiftableBuffer(const uint8_t* aBuffer, uint32_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
	if( aBuffer)
	{
		memcpy( mpBuffer, aBuffer, aSize) ;
		mWritePos += aSize ;
	}
}


void ShiftableBuffer::Shift()
{
	//copy all unread data to the front of the buffer
	memmove(mpBuffer, this->ReadBuff(), this->NumReadBytes());
	mWritePos = this->NumReadBytes();
	mReadPos = 0;
}

void ShiftableBuffer::Reset()
{
	mWritePos = 0;
	mReadPos = 0;
}

ShiftableBuffer::~ShiftableBuffer()
{
	delete[] mpBuffer;
}


void ShiftableBuffer::AdvanceRead(size_t aNumBytes)
{
	assert(aNumBytes <= this->NumReadBytes());

	mReadPos += aNumBytes;
}

void ShiftableBuffer::AdvanceWrite(size_t aNumBytes)
{
	assert(aNumBytes <= this->NumWriteBytes());
	mWritePos += aNumBytes;
}

bool ShiftableBuffer::Sync(const uint8_t* apPattern, size_t aNumBytes)
{
	assert(aNumBytes > 0);

	size_t offset = SyncSubsequence(apPattern, aNumBytes, 0);
	bool res = (this->NumReadBytes() - offset) >= aNumBytes;
	if(offset > 0) this->AdvanceRead(offset);

	return res;
}

size_t ShiftableBuffer::SyncSubsequence(const uint8_t* apPattern, size_t aNumPatternBytes, size_t aOffset)
{
	size_t read_bytes = this->NumReadBytes() - aOffset;
	if(aNumPatternBytes > read_bytes) aNumPatternBytes = read_bytes;

	const uint8_t* pRead = this->ReadBuff() + aOffset;

	for(size_t i = 0; i < aNumPatternBytes; ++i)
	{
		if(apPattern[i] != pRead[i])
			return SyncSubsequence(apPattern, aNumPatternBytes, aOffset + 1);
	}

	return aOffset;
}

}

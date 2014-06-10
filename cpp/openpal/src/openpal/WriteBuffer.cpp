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
#include "WriteBuffer.h"

#include "Comparisons.h"
#include "ReadOnlyBuffer.h"

#include <cstring>

namespace openpal
{

WriteBuffer WriteBuffer::Empty()
{
	return WriteBuffer();
}

WriteBuffer::WriteBuffer(const WriteBuffer& copy) : 
	HasSize(copy),
	pBuffer(copy.pBuffer)
{}

WriteBuffer::WriteBuffer(): 
	HasSize(0),
	pBuffer(nullptr)
{}

WriteBuffer::WriteBuffer(uint8_t* pBuffer_, uint32_t size) :
	HasSize(size),
	pBuffer(pBuffer_)
{}

uint32_t WriteBuffer::ReadFrom(const ReadOnlyBuffer& buffer)
{
	auto num = openpal::Min(buffer.Size(), size);
	memcpy(pBuffer, buffer, num);
	this->Advance(num);
	return num;
}

void WriteBuffer::Clear()
{
	pBuffer = nullptr;
	size = 0;
}

uint32_t WriteBuffer::Advance(uint32_t count)
{
	auto num = openpal::Min(count, size);
	pBuffer += num;
	size -= num;
	return num;
}

ReadOnlyBuffer WriteBuffer::ToReadOnly() const
{
	return ReadOnlyBuffer(pBuffer, size);
}

}



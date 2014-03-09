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
#ifndef __RANGE_WRITE_ITERATOR_H_
#define __RANGE_WRITE_ITERATOR_H_

#include <openpal/BufferWrapper.h>
#include <openpal/ISerializer.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template <class IndexType, class WriteType>
class RangeWriteIterator
{
public:

	static RangeWriteIterator Null()
	{
		auto buffer = openpal::WriteBuffer::Empty();
		return RangeWriteIterator(0, nullptr, buffer);
	}

	RangeWriteIterator(typename IndexType::Type aStart, openpal::ISerializer<WriteType>* pSerializer_, openpal::WriteBuffer& aPosition) :
		start(aStart),
		pSerializer(pSerializer_),
		count(0),
		range(aPosition),
		position(aPosition),
		isNull(aPosition.Size() < 2 * IndexType::Size || pSerializer == nullptr)
	{
		if(!isNull)
		{
			IndexType::WriteBuffer(range, aStart);
			position.Advance(2 * IndexType::Size);
		}
	}

	bool Complete()
	{
		if(isNull || count == 0) return false;
		else
		{
			auto stop = start + count - 1;
			IndexType::Write(range, stop);
			return true;
		}
	}

	bool Write(const WriteType& value)
	{
		if(isNull || position.Size() < pSerializer->Size()) return false;
		else
		{
			pSerializer->Write(value, position);
			++count;
			return true;
		}
	}

	bool IsNull() const
	{
		return isNull;
	}

private:

	typename IndexType::Type start;
	openpal::ISerializer<WriteType>* pSerializer;
	typename IndexType::Type count;

	bool isNull;

	openpal::WriteBuffer range;  // make a copy to record where we write the range
	openpal::WriteBuffer& position;
};

}

#endif

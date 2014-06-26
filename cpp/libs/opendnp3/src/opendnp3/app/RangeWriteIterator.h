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

#include <openpal/container/WriteBuffer.h>
#include <openpal/serialization/ISerializer.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template <class IndexType, class WriteType>
class RangeWriteIterator
{
public:

	static RangeWriteIterator Null()
	{		
		return RangeWriteIterator();
	}

	RangeWriteIterator() : start(0), pSerializer(nullptr), count(0), isValid(false), pPosition(nullptr)
	{}

	RangeWriteIterator(typename IndexType::Type start_, openpal::ISerializer<WriteType>& serializer, openpal::WriteBuffer& position) :
		start(start_),
		pSerializer(&serializer),
		count(0),				
		isValid(position.Size() >= 2 * IndexType::Size),
		range(position),
		pPosition(&position)
	{
		if (isValid)
		{
			IndexType::WriteBuffer(range, start);
			pPosition->Advance(2 * IndexType::Size);
		}
	}

	~RangeWriteIterator()
	{
		if (isValid && count > 0)
		{
			auto stop = start + count - 1;
			IndexType::Write(range, stop);					
		}		
	}	

	bool Write(const WriteType& value)
	{
		if (isValid && pPosition->Size() >= pSerializer->Size())
		{
			pSerializer->Write(value, *pPosition);
			++count;
			return true;			
		}
		else
		{
			return false;
		}
	}

	bool IsValid() const
	{
		return isValid;
	}

private:

	typename IndexType::Type start;
	openpal::ISerializer<WriteType>* pSerializer;
	typename IndexType::Type count;

	bool isValid;

	openpal::WriteBuffer range;  // make a copy to record where we write the range
	openpal::WriteBuffer* pPosition;
};

}

#endif

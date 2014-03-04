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
#ifndef __ITERABLE_BUFFER_H_
#define __ITERABLE_BUFFER_H_

#include <functional>

#include <openpal/BufferWrapper.h>

namespace opendnp3
{

class BufferWithCount
{
	template <class T> friend class IterableTransforms;

	protected:

	BufferWithCount(const openpal::ReadOnlyBuffer& aBuffer, uint32_t aCount) : 
		count(aCount),
		buffer(aBuffer)
	{}

	public:

	uint32_t Count() const { return count; }
	bool IsEmpty() const { return count == 0; }
	bool NonEmpty() const { return count != 0; }

	protected:

	uint32_t count;
	openpal::ReadOnlyBuffer buffer;
};

template <class T>
class IterableBuffer : public BufferWithCount
{
	public:

		template <class U> friend class IterableTransforms;

		IterableBuffer(const openpal::ReadOnlyBuffer& aBuffer, uint32_t aSize) : BufferWithCount(aBuffer, aSize)
		{}
		
		bool ReadOnlyValue(T& value) const
		{			
			if(count == 1) 
			{
				openpal::ReadOnlyBuffer copy(this->buffer);
				value = ValueAt(copy, 0);
				return true;
			}
			else return false;
		}

		template <class IterFunc>
		void foreach(const IterFunc& fun) const
		{
			openpal::ReadOnlyBuffer copy(this->buffer);  // iterate over a mutable copy of the buffer
			for(uint32_t pos = 0; pos < count; ++pos) fun(ValueAt(copy, pos));			
		}

		template <class T, class IterFunc>
		T foldLeft(T seed, const IterFunc& fun) const
		{
			openpal::ReadOnlyBuffer copy(this->buffer);  // iterate over a mutable copy of the buffer
			for(uint32_t pos = 0; pos < count; ++pos)
			{			
				seed = fun(seed, ValueAt(copy, pos));
			}
			return seed;
		}
		
	protected:

		virtual T ValueAt(openpal::ReadOnlyBuffer&, uint32_t pos) const = 0;		
};


}

#endif

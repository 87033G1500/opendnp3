/*
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
#ifndef OPENDNP3_OCTETDATA_H
#define OPENDNP3_OCTETDATA_H

#include <cstdint>

#include <openpal/container/RSlice.h>
#include <openpal/container/StaticBuffer.h>

namespace opendnp3
{

/**
* A base-class for bitstrings containing up to 255 bytes
*/
class OctetData
{
public:

	const static uint8_t MAX_SIZE = 255;

	/**
	* Construct with default length of 1
	*/
	OctetData();

	/**
	* Construct from a c-style string
	*
	* strlen() is used internally to determine the length
	*
	* If the length is 0, it is defaulted to 1
	* If the length is > 255, the first 255 bytes are copied.
	*
	* The null terminator is NOT copied as part of buffer
	*/
	OctetData(const char* input);

	/**
	* Construct from read-only buffer slice
	*
	*
	* If the length is 0, it is defaulted to 1
	* If the length is > 255, the first 255 bytes are copied.
	*
	*/
	OctetData(const openpal::RSlice& input);

	inline uint8_t Size() const
	{
		return size;
	}

	/**
	* Set the buffer to the input buffer/length if the input has length in the interval [1,255]
	*
	* @param input the input data to copy into this object
	*
	* @return true if the input meets the length requirements, false otherwise
	*/
	bool Set(const openpal::RSlice& input);

	/**
	* Set the buffer equal to the supplied c-string if the input string has length in the interval [1,255]
	*
	* @param input c-style string to copy into this object
	*
	* @return true if the input meets the length requirements, false otherwise
	*/
	bool Set(const char* input);

	/**
	* @return a view of the current data as a read-only slice
	*/
	openpal::RSlice ToRSlice() const;

private:

	static openpal::RSlice ToSlice(const char* input);

	openpal::StaticBuffer<MAX_SIZE> buffer;
	uint8_t size;
};

}



#endif


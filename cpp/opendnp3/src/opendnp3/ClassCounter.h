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
#ifndef __CLASS_COUNTER_H_
#define __CLASS_COUNTER_H_

#include <stddef.h>
#include <assert.h>

#include <opendnp3/PointClass.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

/** Utility class that keeps class event counters accessible by enumeration.
*/
class DLL_LOCAL ClassCounter
{
public:

	ClassCounter() : mNumClass1(0), mNumClass2(0), mNumClass3(0) {};
	~ClassCounter() {};

	inline size_t GetNum(PointClass aClass) {
		switch(aClass) {
		case(PC_CLASS_1):
			return mNumClass1;
		case(PC_CLASS_2):
			return mNumClass2;
		case(PC_CLASS_3):
			return mNumClass3;
		case(PC_ALL_EVENTS):
			return mNumClass1 + mNumClass2 + mNumClass3;
		default:
			return 0;
		}
	}

	size_t GetNumAllClass() {
		return mNumClass1 + mNumClass2 + mNumClass3;
	}

	void IncrCount(PointClass aClass);
	void DecrCount(PointClass aClass);

private:

	size_t mNumClass1;
	size_t mNumClass2;
	size_t mNumClass3;

};

}


#endif


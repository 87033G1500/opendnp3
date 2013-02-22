
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __DNP_DATABASE_TYPES_H_
#define __DNP_DATABASE_TYPES_H_

#include <opendnp3/Types.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/PointClass.h>
#include <opendnp3/Visibility.h>

#include "VtoData.h"

#include <vector>

namespace opendnp3
{

/**
 * Common base class for point information and associated meta-data needed for
 * processing it.  The common base class allows some common operations to be
 * performed on static and event types.
 */
template<typename T>
struct DLL_LOCAL PointInfoBase {
	PointInfoBase(const T& arValue, PointClass aClass, size_t aIndex) :
		mValue(arValue),
		mClass(aClass),
		mIndex(aIndex)
	{}

	PointInfoBase() :
		mClass(PC_CLASS_0),
		mIndex(0)
	{}

	T mValue;			// current measurment (i.e. Binary, Analog, etc)
	PointClass mClass;	// class of the point (PC_CLASS<0-3>)
	size_t mIndex;		// index of the measurement

	typedef T MeasType;
};

/**
 * Structure for holding static data information. Adds a deadband and a last
 * event value to the base class.
 */
template<typename T>
struct DLL_LOCAL PointInfo : public PointInfoBase<T> {
	PointInfo(const T& arVal, PointClass aClass, size_t aIndex) :
		PointInfoBase<T>(arVal, aClass, aIndex),
		mDeadband(0),
		mLastEventValue(0),
		mSequence(0)
	{}

	PointInfo() :
		mDeadband(0),
		mLastEventValue(0),
		mSequence(0)
	{}

	double mDeadband;						// deadband associated with measurement (optional)
	typename T::ValueType mLastEventValue;	// the last value that was reported
	size_t mSequence;
};

template <class DataInfoType>
struct DLL_LOCAL StaticIter {
	typedef typename std::vector< DataInfoType >::const_iterator Type;
};

typedef PointInfo<Binary>				BinaryInfo;
typedef PointInfo<Analog>				AnalogInfo;
typedef PointInfo<Counter>				CounterInfo;
typedef PointInfo<ControlStatus>			ControlStatusInfo;
typedef PointInfo<SetpointStatus>			SetpointStatusInfo;
typedef PointInfoBase<VtoData>				VtoDataInfo;

typedef StaticIter<BinaryInfo>::Type			BinaryIterator;
typedef StaticIter<AnalogInfo>::Type			AnalogIterator;
typedef StaticIter<CounterInfo>::Type			CounterIterator;
typedef StaticIter<ControlStatusInfo>::Type		ControlIterator;
typedef StaticIter<SetpointStatusInfo>::Type		SetpointIterator;

} //end namespace

/* vim: set ts=4 sw=4: */

#endif


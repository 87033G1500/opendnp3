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
#ifndef __MEASUREMENT_HELPERS_H_
#define	__MEASUREMENT_HELPERS_H_

#include "opendnp3/app/BaseMeasurementTypes.h"
#include <openpal/util/ToHex.h>

namespace opendnp3
{

/* TODO
template <class T>
std::string ToString(const TypedMeasurement<T>& arMeas)
{
	std::ostringstream oss;
	oss << "Value: " << arMeas.GetValue() << " Quality: 0x" << openpal::ByteToHex(arMeas.GetQuality()) << " Time: " << arMeas.GetTime();
	return oss.str();
}
*/

}

#endif

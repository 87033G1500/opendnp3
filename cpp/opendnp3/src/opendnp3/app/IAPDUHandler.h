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
#ifndef __I_APDU_HANDLER_H_
#define __I_APDU_HANDLER_H_

#include "opendnp3/app/GroupVariation.h"

#include "opendnp3/app/IterableBuffer.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/IndexedValue.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/app/StaticRange.h"

#include "opendnp3/gen/QualifierCode.h"

#include "opendnp3/objects/Group50.h"
#include "opendnp3/objects/Group51.h"
#include "opendnp3/objects/Group52.h"

namespace opendnp3
{

/**
* Second stage of dealing with 
*/
class IAPDUHandler 
{
	public:

		virtual void AllObjects(GroupVariation gv) = 0;

		virtual void OnRangeRequest(GroupVariation gv, const StaticRange& range) = 0;

		virtual void OnCountRequest(GroupVariation gv, uint32_t count) = 0;

		virtual void OnCountOf(const IterableBuffer<Group50Var1>& times) = 0;

		virtual void OnCountOf(const IterableBuffer<Group51Var1>& times) = 0;

		virtual void OnCountOf(const IterableBuffer<Group51Var2>& times) = 0;

		virtual void OnCountOf(const IterableBuffer<Group52Var2>& times) = 0;

		virtual void OnIIN(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<bool, uint16_t>>& meas) = 0;

		// static values
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) = 0;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) = 0;

		// events - 16 bit indices
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) = 0;		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) = 0;

		// events - 8 bit indices
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint8_t>>& meas) = 0;

		// commmands - 16 bit indices
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) = 0;

		// commmands - 8 bit indices
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas) = 0;
						
};

}

#endif

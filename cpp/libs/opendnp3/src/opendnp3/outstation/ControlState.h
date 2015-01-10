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
#ifndef OPENDNP3_CONTROLSTATE_H
#define OPENDNP3_CONTROLSTATE_H


#include <openpal/executor/MonotonicTimestamp.h>


namespace opendnp3
{

///
/// Represent all of the mutable state for SBO controls
///
class ControlState
{
	
	public:	
	
	ControlState() : 
		rxFragCount(0),
		operateExpectedSeq(0),
		operateExpectedFragCount(0)
	{}

	// Is this sequence number what's expected for the OPERATE
	bool IsOperateSequenceValid(uint8_t seqN) const
	{
		return (rxFragCount == operateExpectedFragCount) && (seqN == operateExpectedSeq);
	}

	
	openpal::MonotonicTimestamp selectTime;	
	uint32_t rxFragCount;
	uint8_t operateExpectedSeq;
	uint32_t operateExpectedFragCount;	
};


}

#endif

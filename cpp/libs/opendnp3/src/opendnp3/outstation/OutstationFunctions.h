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
#ifndef OPENDNP3_OUTSTATIONFUNCTIONS_H
#define OPENDNP3_OUTSTATIONFUNCTIONS_H

#include <openpal/container/Pair.h>
#include <openpal/util/Uncopyable.h>

#include "opendnp3/outstation/OutstationState.h"

namespace opendnp3
{

/// Reusable actions that operate on oustation state
class OFunctions : private openpal::PureStatic
{
	
	public:			

	/// Handles non-read function codes that require a response. builds the response using the supplied writer.
	/// @return An IIN field indicating the validity of the request, and to be returned in the response.
	static IINField HandleNonReadResponse(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	
	/// Handles read function codes. May trigger an unsolicited response	
	/// @return an IIN field and a partial AppControlField (missing sequence info)
	static openpal::Pair<IINField, AppControlField> HandleRead(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);

	/// Handles no-response function codes.
	static void HandleNoResponseFunction(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	private:

	// ------ Function Handlers ------

	static IINField HandleWrite(OState& ostate, const openpal::ReadBufferView& objects);
	static IINField HandleSelect(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	static IINField HandleOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	static IINField HandleDirectOperate(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter* pWriter);
	static IINField HandleDelayMeasure(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	static IINField HandleRestart(OState& ostate, const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter);
	static IINField HandleAssignClass(OState& ostate, const openpal::ReadBufferView& objects);
	static IINField HandleDisableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	static IINField HandleEnableUnsolicited(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer);
	static IINField HandleCommandWithConstant(OState& ostate, const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status);
};


}



#endif


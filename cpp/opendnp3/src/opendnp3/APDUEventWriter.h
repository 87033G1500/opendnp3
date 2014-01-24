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

#ifndef __APDU_EVENT_WRITER_H_
#define __APDU_EVENT_WRITER_H_

#include "IEventWriter.h"
#include "IEventWriterState.h"
#include "BinaryWriteState.h"

#include "objects/Group2.h"

#include <openpal/Serialization.h>

namespace opendnp3
{

class APDUEventWriter : public IEventWriter
{
	public:

	APDUEventWriter();
				
	bool Write(const Event<Binary>& evt) final;
	bool Write(const Event<Analog>& evt) final;
	bool Write(const Event<Counter>& evt) final;

	private:

	// current state
	IEventWriterState* pState;

	// states
	InitialEventWriterState initial;

	BinaryWriteState<openpal::UInt16, Group2Var1> stateGroup2Var1;
	BinaryWriteState<openpal::UInt16, Group2Var2> stateGroup2Var2;

};


}

#endif

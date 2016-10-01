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

#ifndef OPENDNP3_STACKPAIR_H
#define OPENDNP3_STACKPAIR_H

#include "asiodnp3/DNP3Manager.h" 
#include "opendnp3/LogLevels.h"

#include "QueuingSOEHandler.h"

#include <memory>

namespace asiodnp3 {

	class StackPair final : openpal::Uncopyable
	{			
		const uint16_t NUM_POINTS_PER_TYPE;
		const uint32_t EVENTS_PER_ITERATION;
		const std::shared_ptr<opendnp3::QueuingSOEHandler> soeHandler;

		static const uint32_t LEVELS = opendnp3::levels::NORMAL;

		IMaster* const master;
		IOutstation* const outstation;
	
		static OutstationStackConfig GetOutstationStackConfig(uint16_t numPointsPerType, uint16_t eventBufferSize);
		static MasterStackConfig GetMasterStackConfig();

		static IMaster* CreateMaster(DNP3Manager&, uint16_t port, std::shared_ptr<opendnp3::ISOEHandler>);
		static IOutstation* CreateOutstation(DNP3Manager&, uint16_t port, uint16_t numPointsPerType, uint16_t eventBufferSize);

		static std::string GetId(const char* name, uint16_t port);

	public:

		StackPair(DNP3Manager&, uint16_t port, uint16_t numPointsPerType, uint32_t eventsPerIteration);

		
	};

}

#endif


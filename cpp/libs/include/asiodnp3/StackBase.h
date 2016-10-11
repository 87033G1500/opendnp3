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
#ifndef ASIODNP3_STACKBASE_H
#define ASIODNP3_STACKBASE_H

#include "asiodnp3/IStack.h"
#include "asiopal/StrandExecutor.h"
#include "asiodnp3/IOHandler.h"
#include "opendnp3/transport/TransportStack.h"

namespace asiodnp3
{

/**
* Base class for masters or outstations
*/
class StackBase
{
public:


	StackBase(
		const openpal::Logger& logger, 
		const std::shared_ptr<asiopal::StrandExecutor>& executor, 
		const std::shared_ptr<opendnp3::ILinkListener>& listener,
		const std::shared_ptr<IOHandler>& iohandler,
		uint32_t maxRxFragSize,
		const opendnp3::LinkConfig& config);

	bool Enable();

	bool Disable();

	void Shutdown();

	opendnp3::StackStatistics GetStackStatistics();


	// members

	openpal::Logger logger;
	opendnp3::StackStatistics statistics;
	const std::shared_ptr<asiopal::StrandExecutor> executor;
	const std::shared_ptr<IOHandler> iohandler;
	opendnp3::TransportStack tstack;

};


}

#endif

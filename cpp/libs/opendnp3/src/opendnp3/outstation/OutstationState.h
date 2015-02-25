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
#ifndef OPENDNP3_OUTSTATIONSTATE_H
#define OPENDNP3_OUTSTATIONSTATE_H

#include "opendnp3/LayerInterfaces.h"

#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/outstation/RequestHistory.h"
#include "opendnp3/outstation/DeferredRequest.h"
#include "opendnp3/outstation/OutstationChannelStates.h"
#include "opendnp3/outstation/ControlState.h"
#include "opendnp3/outstation/OutstationSeqNum.h"
#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/EventBuffer.h"
#include "opendnp3/outstation/ResponseContext.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/IOutstationAuthProvider.h"

#include <openpal/executor/TimerRef.h>
#include <openpal/logging/LogRoot.h>

namespace opendnp3
{

///
/// Represent all of the mutable state in an outstation
///
class OState
{
	
	public:		

	OState(	const OutstationConfig& config,	
			const DatabaseTemplate& dbTemplate,
			openpal::Logger logger,						
			openpal::IExecutor& executor,			
			ILowerLayer& lower,
			ICommandHandler& commandHandler,
			IOutstationApplication& application,
			IOutstationAuthProvider& authProvider);

	// reset important variables to their initial state
	void Reset();	

	// returns true if the layer is online and not transmitting
	bool CanTransmit() const;

	// ------ resources --------
	openpal::Logger logger;
	openpal::IExecutor* const pExecutor;
	ILowerLayer* const pLower;	
	ICommandHandler* const pCommandHandler;
	IOutstationApplication* const pApplication;
	IOutstationAuthProvider* const pAuthProvider;

	// ------ Database, event buffer, and response tracking
	EventBuffer eventBuffer;
	Database database;
	ResponseContext rspContext;

	// ------ Static configuration -------
	OutstationParams params;	
		
	// ------ Shared dynamic state --------
	bool isOnline;
	bool isTransmitting;	
	IINField staticIIN;
	openpal::TimerRef confirmTimer;
	RequestHistory history;
	DeferredRequest deferred;

	// ------ Dynamic state related to controls ------
	ControlState control;

	// ------ Dynamic state related to solicited and unsolicited modes ------			
	OutstationSolState  sol;
	OutstationUnsolState unsol;
};


}

#endif

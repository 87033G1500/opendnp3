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
#ifndef SECAUTHV5_SECURITYSTATE_H
#define SECAUTHV5_SECURITYSTATE_H

#include <opendnp3/gen/KeyStatus.h>
#include <opendnp3/outstation/DeferredRequest.h>
#include <opendnp3/outstation/TxBuffer.h>

#include <openpal/executor/IExecutor.h>
#include <openpal/crypto/ICryptoProvider.h>

#include "secauthv5/IUserDatabase.h"

#include "KeyChangeState.h"
#include "OutstationAuthSettings.h"

namespace secauthv5
{

class IOAuthState;

class SecurityState
{
	public:

	SecurityState(const OutstationAuthSettings& settings, openpal::Logger logger, openpal::IExecutor& executor, IUserDatabase& userDatabase, openpal::ICryptoProvider& crypto);

	void Reset();
	
	opendnp3::DeferredRequest deferred;
	openpal::IExecutor* pExecutor;
	IUserDatabase* pUserDatabase;
	openpal::ICryptoProvider* pCrypto;
	opendnp3::KeyStatus keyStatus;
	IOAuthState* pState;
	KeyChangeState keyChangeState;
	opendnp3::TxBuffer txBuffer;
};

}

#endif


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

#include "OutstationAuthStack.h"

using namespace secauth;

namespace asiodnp3
{

	OutstationAuthStack::OutstationAuthStack(
		const char* id,
		openpal::LogRoot& root_,
		openpal::IExecutor& executor,
		opendnp3::ICommandHandler& commandHandler,
		opendnp3::IOutstationApplication& application,
		const opendnp3::OutstationStackConfig& config,
		const StackActionHandler& handler,
		const OutstationAuthSettings& authSettings,
		openpal::IUTCTimeSource& timeSource,
		secauth::IUserDatabase& userDB,
		openpal::ICryptoProvider& crypto) :
		OutstationStackImpl(id, root_, executor, commandHandler, application, config, handler),
		auth(authSettings, root.GetLogger(), executor, timeSource, userDB, crypto)
	{
		// configure the authentication
		this->outstation.SetAuthProvider(auth);
	}

}




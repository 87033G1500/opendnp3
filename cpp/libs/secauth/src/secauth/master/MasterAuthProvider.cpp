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

#include "MasterAuthProvider.h"


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

MasterAuthProvider::MasterAuthProvider(
	opendnp3::IMasterApplication& application,
	openpal::IExecutor& executor,
	openpal::Logger logger,
	openpal::ICryptoProvider& crypto,
	IMasterUserDatabase& userDB
	) : 
	msstate(application, executor, crypto, userDB),
	sessionKeyTask(application, TimeDuration::Seconds(5), logger, User::Default(), msstate)
{

}

openpal::ManagedPtr<opendnp3::IMasterTask> MasterAuthProvider::TryStartTask()
{
	if (sessionKeyTask.ExpirationTime().IsMax())
	{
		return openpal::ManagedPtr<IMasterTask>();
	}
	else
	{
		return openpal::ManagedPtr<IMasterTask>::WrapperOnly(&sessionKeyTask);
	}	
}

void MasterAuthProvider::GoOnline(opendnp3::MState& mstate)
{
	
}

void MasterAuthProvider::GoOffline(opendnp3::MState& mstate)
{
	// TODO reset the sessions?
}

}


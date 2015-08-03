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
#ifndef ASIODNP3_IMASTERSA_H
#define ASIODNP3_IMASTERSA_H

#include "IMaster.h"

#include <opendnp3/app/User.h>
#include <secauth/UpdateKey.h>

#include <secauth/master/UserStatusChange.h>
#include <secauth/master/BeginUpdateKeyChangeCallbackT.h>

namespace asiodnp3
{

/** 
* @desc Extends the vanilla DNP3 master interface for secure authentication
*/
class IMasterSA : public IMaster
{
public:		

	virtual ~IMasterSA() {}

	/**
	* Begins a users status change operation. The master just acts as a pass through for this operation
	*/
	virtual void ChangeUserStatus(const secauth::UserStatusChange& userStatusChange,  const opendnp3::TaskConfig& config = opendnp3::TaskConfig::Default()) = 0;

	/**
	*  Begins the first step in changing a user's update key
	*/
	virtual void BeginUpdateKeyChange(opendnp3::KeyChangeMethod, const std::string& userName, const opendnp3::TaskConfig& config, const secauth::BeginUpdateKeyChangeCallbackT& handler) = 0;
	
	/**
	* Add a user to the outstation. This is normally only done during initialization.
	*/
	virtual void AddUser(opendnp3::User user, const secauth::UpdateKey& key) = 0;
};

}

#endif


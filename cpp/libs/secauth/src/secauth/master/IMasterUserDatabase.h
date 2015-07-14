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

#ifndef SECAUTH_IMASTERUSERDATABASE_H
#define SECAUTH_IMASTERUSERDATABASE_H

#include <opendnp3/gen/UpdateKeyMode.h>
#include <opendnp3/app/User.h>

#include <openpal/container/ReadBufferView.h>

#include <functional>

namespace secauth
{	

/** 
	An interface for retrieving info about configured users on the master	
*/
class IMasterUserDatabase
{
	public:		

		virtual void EnumerateUsers(const std::function<void(const opendnp3::User)>& fun) const = 0;

		virtual bool GetUpdateKey(const opendnp3::User& user, opendnp3::UpdateKeyMode& type, openpal::ReadBufferView& key) const = 0;

		virtual bool UserExists(const opendnp3::User& user) const = 0;
};

}

#endif


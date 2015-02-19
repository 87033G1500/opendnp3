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
#ifndef SECAUTHV5_SESSIONSTORE_H
#define SECAUTHV5_SESSIONSTORE_H

#include "secauthv5/SessionKeys.h"
#include "secauthv5/IUserDatabase.h"
#include "secauthv5/User.h"

#include <opendnp3/gen/KeyStatus.h>
#include <openpal/executor/IMonotonicTimeSource.h>

#include <map>
#include <memory>

namespace secauthv5
{
	// models the state of a session for a particular user
	class SessionState
	{
		public:

		SessionState();

		opendnp3::KeyStatus status;
		SessionKeys keys;
		openpal::MonotonicTimestamp expirationTime;
		uint32_t authCount;
		uint32_t authCountMax;
		openpal::ReadBufferView lastKeyUpdateHMAC;

		private:
		openpal::StaticBuffer<AuthConstants::MAX_HMAC_SIZE> hmacBuffer;
	};


	/// Stores session key info for all users
	class SessionStore : private openpal::Uncopyable
	{
		public:

		SessionStore(			
			openpal::IMonotonicTimeSource& timeSource,
			IUserDatabase& userdb
		);

		// Session keys are only set if KeyStatus == OK
		opendnp3::KeyStatus GetSessionKeys(const User& user, SessionKeysView& view);
		
		opendnp3::KeyStatus GetKeyStatus(const User& user, openpal::ReadBufferView& lastKeyChangeHMAC);

		// Increments the auth count for the specified users session keys
		// this may invalidate the session keys
		opendnp3::KeyStatus IncrementAuthCount(const User& user);

		private:

		opendnp3::KeyStatus CheckTimeValidity(SessionState& state);

		opendnp3::KeyStatus IncrementAuthCount(SessionState& state);

		openpal::IMonotonicTimeSource* pTimeSource;

		std::map<uint16_t, std::unique_ptr<SessionState>> sessionMap;
	};
}

#endif


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
#ifndef SECAUTH_MASTERAUTHCONTEXT_H
#define SECAUTH_MASTERAUTHCONTEXT_H

#include <opendnp3/master/MasterContext.h>

#include <openpal/util/Uncopyable.h>

#include "secauth/UpdateKey.h"
#include "secauth/master/SessionKeyTask.h"
#include "secauth/master/MasterAuthSettings.h"
#include "secauth/master/IAuthResponseReceiver.h"
#include "secauth/master/MasterUserDatabase.h"

namespace secauth
{

class MAuthContext final : public opendnp3::MContext, private IAuthResponseReceiver
{	

public:	

	MAuthContext(
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		opendnp3::ILowerLayer& lower,
		opendnp3::ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		const opendnp3::MasterParams& params,
		opendnp3::ITaskLock& taskLock,
		const secauth::MasterAuthSettings& authSettings,
		openpal::ICryptoProvider& crypto		
	);


	// ------ override the necessary MContext methods ------		

	virtual bool GoOnline() override;

	virtual bool GoOffline() override;

	virtual void OnParsedHeader(const openpal::ReadBufferView& apdu, const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects) override final;

	virtual bool CanRun(const opendnp3::IMasterTask& task) override final;

	virtual void RecordLastRequest(const openpal::ReadBufferView& apdu) override final;

	// --- public helpers ----

	bool AddUser(opendnp3::User user, const secauth::UpdateKey& key);

private:

	void OnReceiveAuthResponse(const openpal::ReadBufferView& apdu, const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects);

	// ------ Implement IAuthResponseReceiver ------	

	virtual void OnAuthChallenge(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;

	virtual void OnAuthError(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const opendnp3::Group120Var7& error) override final;
	
	typedef std::map<uint16_t, std::unique_ptr<SessionKeyTask>> SessionKeyTaskMap;
	
	MasterAuthSettings			settings;
	openpal::IUTCTimeSource*	pTimeSource;
	openpal::ICryptoProvider*	pCrypto;
	MasterUserDatabase		    userDB;
	SessionStore				sessions;		
	openpal::ReadBufferView		lastRequest;
	SessionKeyTaskMap			sessionKeyTaskMap;

	openpal::StaticBuffer<AuthSizes::MAX_MASTER_CHALLENGE_REPLY_FRAG_SIZE> challengeReplyBuffer;

};


}

#endif

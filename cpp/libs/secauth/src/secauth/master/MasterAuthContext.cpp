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

#include "MasterAuthContext.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/master/MasterContext.h>

#include <openpal/logging/LogMacros.h>

#include "secauth/Crypto.h"
#include "secauth/HMACProvider.h"
#include "secauth/SingleObjectHandlers.h"

#include "secauth/master/UserStatusChangeTask.h"
#include "secauth/master/BeginUpdateKeyChangeTask.h"
#include "secauth/master/FinishUpdateKeyChangeTask.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/app/parsing/ObjectHeaderParser.h"




using namespace openpal;
using namespace opendnp3;

namespace secauth
{

MAuthContext::MAuthContext(
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		opendnp3::ILowerLayer& lower,
		opendnp3::ISOEHandler& SOEHandler,
		secauth::IMasterApplicationSA& application,
		const opendnp3::MasterParams& params,
		opendnp3::ITaskLock& taskLock,
		const secauth::MasterAuthSettings& authSettings,
		openpal::ICryptoProvider& crypto		
	) : 
	MContext(executor, root, lower, SOEHandler, application, params, taskLock),	
	security(executor, application, authSettings, crypto)
{

}

bool MAuthContext::OnLowerLayerUp()
{
	bool ret = MContext::OnLowerLayerUp();

	if (ret)
	{
		// create a session key task for every user
		auto createSessionKeyTask = [this](const User& user)
		{				
			auto task = std::unique_ptr<SessionKeyTask>(
				new SessionKeyTask(*this->pApplication, this->params.taskRetryPeriod, security.settings.sessionChangeInterval, this->logger, user, *security.pCrypto, security.userDB, security.sessions)
			);			
			
			this->scheduler.Schedule(openpal::ManagedPtr<IMasterTask>::WrapperOnly(task.get()));

			security.sessionKeyTaskMap[user.GetId()] = std::move(task);
		};

		security.userDB.EnumerateUsers(createSessionKeyTask);
	}
		
	return ret;
}

bool MAuthContext::OnLowerLayerDown()
{	
	auto ret = MContext::OnLowerLayerDown();

	if (ret)
	{
		security.sessionKeyTaskMap.clear();
		security.sessions.Clear();
	}

	return ret;
}

void MAuthContext::OnParsedHeader(const openpal::ReadBufferView& apdu, const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{	
	switch (header.function)
	{
		case(FunctionCode::AUTH_RESPONSE) :
			this->OnReceiveAuthResponse(apdu, header, objects);
			break;
		case(FunctionCode::RESPONSE) :
			this->ProcessResponse(header, objects);
			break;
		case(FunctionCode::UNSOLICITED_RESPONSE) :
			this->ProcessUnsolicitedResponse(header, objects);
			break;
		default:
			FORMAT_LOG_BLOCK(this->logger, opendnp3::flags::WARN, "Ignoring unsupported function code: %s", FunctionCodeToString(header.function));
			break;
	}	
}

bool MAuthContext::CanRun(const opendnp3::IMasterTask& task)
{
	if (task.IsAuthTask())
	{
		return true;
	}

	// is there an active session for the task's user?

	auto status = security.sessions.GetSessionKeyStatus(task.GetUser());

	return status == KeyStatus::OK;
}

bool MAuthContext::MeetsUserRequirements(const IMasterTask& task)
{
	if (task.IsAuthTask())
	{
		return true;
	}

	return security.userDB.UserExists(task.GetUser());
}

void MAuthContext::RecordLastRequest(const openpal::ReadBufferView& apdu)
{
	security.lastRequest = apdu;
}

bool MAuthContext::AddUser(opendnp3::User user, const secauth::UpdateKey& key)
{
	auto ret = security.userDB.AddUser(user, key);
	if (ret) 
	{
		// invalidate any active sessions for this user immediately to force usage of the new update key
		security.sessions.Invalidate(user);
	}
	return ret;
}

void MAuthContext::ChangeUserStatus(const UserStatusChange& userStatusChange, const opendnp3::TaskConfig& config)
{
	auto task = new UserStatusChangeTask(userStatusChange, *security.pApplicationSA, logger, config);
	this->ScheduleAdhocTask(task);
}

void MAuthContext::BeginUpdateKeyChange(const std::string& username, const opendnp3::TaskConfig& config, const BeginUpdateKeyChangeCallbackT& callback)
{
	auto task = new BeginUpdateKeyChangeTask(username, *security.pApplicationSA, logger, config, *security.pCrypto, callback);
	this->ScheduleAdhocTask(task);
}

void MAuthContext::FinishUpdateKeyChange(const FinishUpdateKeyChangeArgs& args, const opendnp3::TaskConfig& config)
{
	auto task = new FinishUpdateKeyChangeTask(args, *security.pApplicationSA, logger, config, security.pCrypto->GetSHA256HMAC());
	this->ScheduleAdhocTask(task);
}

void MAuthContext::OnReceiveAuthResponse(const openpal::ReadBufferView& apdu, const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	// need to determine the context of the auth response
	
	if (!(this->tstate == TaskState::WAIT_FOR_RESPONSE && this->pActiveTask.IsDefined()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring unexpected AuthResponse");
		return;
	}

	// an auth-based task is running and needs to receive this directly
	if (this->pActiveTask->IsAuthTask())
	{
		this->ProcessResponse(header, objects);
		return;
	}

	auto gv = GroupVariation::UNKNOWN;
	if (!ObjectHeaderParser::ReadFirstGroupVariation(objects, gv))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring AuthResponse with bad first header");
		return;
	}

	switch (gv)
	{
		case(GroupVariation::Group120Var1) : // a challenge
			this->OnAuthChallenge(apdu, header, objects);
			return;
		case(GroupVariation::Group120Var7) : // an error
			this->OnAuthError(apdu, header, objects); 
			return;
		default:
			FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unknown AuthResponse type: %s", GroupVariationToString(gv));
			return;
	}	
}

void MAuthContext::OnAuthChallenge(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (this->isSending)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring authentication challenge while transmitting");
		return;
	}

	ChallengeHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);
	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring malformed challenge");
		return;
	}

	auto& challengeData = handler.value.challengeData;

	if (!AuthSizes::ChallengeDataSizeWithinLimits(challengeData.Size()))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Challenge data size outside of limits: %u", challengeData.Size());
		return;
	}

	if (!pActiveTask.IsDefined())
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "No active task to challenge");
		return;
	}

	auto user = pActiveTask->GetUser();

	// lookup the session keys
	SessionKeysView keys;
	if (security.sessions.TryGetSessionKeys(user, keys) != KeyStatus::OK)
	{		
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Session is not valid for user: %u", user.GetId());
		return;
	}

	HMACMode hmacMode;
	if (!Crypto::TryGetHMACMode(handler.value.hmacAlgo, hmacMode))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Outstation requested unsupported HMAC type: %s", HMACTypeToString(handler.value.hmacAlgo));
		return;
	}	

	HMACProvider hmacProvider(*security.pCrypto, hmacMode);

	std::error_code ec;
	auto hmacValue = hmacProvider.Compute(keys.controlKey, { apdu, security.lastRequest }, ec);

	if (ec)
	{
		FORMAT_LOG_BLOCK(this->logger, flags::ERR, "Error calculating HMAC: %s", ec.message().c_str());
		return;
	}
		
	Group120Var2 challengeReply;
	challengeReply.challengeSeqNum = handler.value.challengeSeqNum;
	challengeReply.userNum = user.GetId();
	challengeReply.hmacValue = hmacValue;
	
	APDURequest reply(security.challengeReplyBuffer.GetWriteBuffer());
	reply.SetFunction(FunctionCode::AUTH_REQUEST);
	reply.SetControl(AppControlField::Request(header.control.SEQ));
	
	if (!reply.GetWriter().WriteFreeFormat(challengeReply))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "Unable to write challenge reply");
		return;
	}

	this->Transmit(reply.ToReadOnly());
}

void MAuthContext::OnAuthError(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ErrorHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);
	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring malformed error");
		return;
	}

	auto errorCode = handler.value.errorCode;

	FORMAT_LOG_BLOCK(this->logger, flags::WARN,
		"Received auth error from outstation w/ code: %s",
		AuthErrorCodeToString(errorCode)
	);

	if (this->tstate != TaskState::WAIT_FOR_RESPONSE || !this->pActiveTask.IsDefined())
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Discarding unexpected auth error");
		return;
	}

	// at this point, we're transitioning back to the master's idle state
	this->tstate = TaskState::IDLE;
	this->responseTimer.Cancel();

	if (errorCode == AuthErrorCode::AUTHORIZATION_FAILED)
	{
		this->pActiveTask->OnAuthorizationFailure(pExecutor->GetTime());
	}
	else
	{
		/// treat all other error codes as authentication failures which could mean a lot of different things
		this->pActiveTask->OnAuthenticationFailure(pExecutor->GetTime());
	}

	this->CompleteActiveTask();
}

}


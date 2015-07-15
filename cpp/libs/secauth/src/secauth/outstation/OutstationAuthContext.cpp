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

#include "OutstationAuthContext.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/outstation/OutstationContext.h"

#include "secauth/AggressiveModeParser.h"
#include "secauth/outstation/KeyUnwrap.h"
#include "secauth/outstation/OAuthStates.h"
#include "secauth/Crypto.h"

#include "AuthRequestHandler.h"
#include "IOAuthState.h"
#include "secauth/StatThresholds.h"


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

DatabaseTemplate OAuthContext::EnableSecStats(const DatabaseTemplate& dbTemplate)
{
	DatabaseTemplate copy(dbTemplate);
	copy.numSecurityStats = StatThresholds::NUM_STATS;
	return copy;
}

void OAuthContext::ConfigureSecStats()
{
	auto stats = this->database.buffers.buffers.GetArrayView<SecurityStat>();
	for (uint16_t i = 0; i < StatThresholds::NUM_STATS; ++i)
	{
		stats[i].value.quality = 0x01;
		uint32_t deadband = StatThresholds::GetDeadband(i);
	}
}

OAuthContext::OAuthContext(
			const OutstationConfig& config,
			const DatabaseTemplate& dbTemplate,
			openpal::Logger logger,
			openpal::IExecutor& executor,
			ILowerLayer& lower,
			ICommandHandler& commandHandler,
			IOutstationApplication& application,
			const OutstationAuthSettings& settings,
			openpal::IUTCTimeSource& timeSource,
			IOutstationUserDatabase& userDatabase,
			openpal::ICryptoProvider& crypto
		) :
		OContext(config, EnableSecStats(dbTemplate), logger, executor, lower, commandHandler, application),
		sstate(config.params, settings, logger, executor, timeSource, userDatabase, crypto)
{
	this->ConfigureSecStats();
}

bool OAuthContext::GoOffline()
{
	if (this->isOnline)
	{
		this->Reset();
	}
	return OContext::GoOffline();
}

void OAuthContext::CheckForTaskStart()
{
	if (this->CanTransmit() && sstate.deferred.IsSet())
	{
		auto handler = [this](const openpal::ReadBufferView& apdu, const APDUHeader& header, const ReadBufferView& objects)
		{
			this->ProcessAuthAPDU(apdu, header, objects);
			return true;
		};

		sstate.deferred.Process(handler);
	}
}
		
void OAuthContext::ReceiveParsedHeader(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (this->CanTransmit())
	{
		this->ProcessAuthAPDU(apdu, header, objects);
	}
	else
	{
		sstate.deferred.SetASDU(header, apdu);
	}
}

void OAuthContext::ProcessAuthAPDU(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
	case(FunctionCode::AUTH_REQUEST) :
		this->OnAuthRequest(apdu, header, objects);
		break;
	case(FunctionCode::AUTH_RESPONSE) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthResponse not valid for outstation");
		break;
	case(FunctionCode::AUTH_REQUEST_NO_ACK) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthRequestNoAck not supported");
		break;
	default:
		this->OnUnknownRequest(apdu, header, objects);
		break;
	}
}

void OAuthContext::OnAuthRequest(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring AuthRequest with UNS bit set");
	}
	else
	{
		AuthRequestHandler handler(apdu, header, *this, this->logger);
		APDUParser::Parse(objects, handler, &this->logger);
	}
}

void OAuthContext::OnUnknownRequest(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	/// We have to determine if this is a regular request or an aggressive mode request
	AggModeResult result = AggressiveModeParser::IsAggressiveMode(objects, &this->logger);
	if (result.result == ParseResult::OK)
	{
		if (result.isAggMode)
		{
			// it's an aggressive mode request
			sstate.pState = sstate.pState->OnAggModeRequest(*this, header, objects, result.request);
		}
		else
		{
			// it's a normal DNP3 request
			sstate.pState = sstate.pState->OnRegularRequest(*this, apdu, header, objects);
		}
	}	
}

void OAuthContext::Reset()
{		
	this->sstate.pState = OAuthStateIdle::Instance();			
}

opendnp3::APDUResponse OAuthContext::StartResponse()
{
	auto response = sstate.txBuffer.Start();
	response.SetIIN(this->GetResponseIIN());
	return response;
}

void OAuthContext::OnAuthChallenge(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var1& challenge)
{	
	sstate.pState = sstate.pState->OnAuthChallenge(*this, header, challenge);
}

void OAuthContext::OnAuthReply(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var2& reply)
{	
	sstate.pState = sstate.pState->OnAuthReply(*this, header, reply);
}

void OAuthContext::OnRequestKeyStatus(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var4& status)
{	
	sstate.pState = sstate.pState->OnRequestKeyStatus(*this, header, status);
}

void OAuthContext::OnChangeSessionKeys(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var6& change)
{
	sstate.pState = sstate.pState->OnChangeSessionKeys(*this, apdu, header, change);
}

void OAuthContext::ProcessChangeSessionKeys(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
{
	User user(change.userNum);

	if (!sstate.keyChangeState.CheckUserAndKSQMatches(user, change.keyChangeSeqNum))
	{
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
		return;
	}

	UpdateKeyMode updateKeyType;
	ReadBufferView updateKey;

	if (!sstate.pUserDatabase->GetUpdateKey(user, updateKeyType, updateKey))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring session key change request for unknown user %u", change.userNum);
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
		return;
	}

	UnwrappedKeyData unwrapped;
	KeyUnwrapBuffer buffer;

	auto unwrapSuccess = buffer.Unwrap(
		GetKeyWrapAlgo(*sstate.pCrypto, updateKeyType),
		updateKey,
		change.keyWrapData,
		unwrapped,
		&this->logger
		);

	if (!unwrapSuccess)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Failed to unwrap key data");
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	if (!sstate.keyChangeState.EqualsLastStatusResponse(unwrapped.keyStatusObject))
	{
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	// At this point, we've successfully authenticated the session key change for this user
	// We compute the HMAC based on the full ASDU and the monitoring direction session key		

	std::error_code ec;
	auto hmac = sstate.hmac.Compute(unwrapped.keys.monitorKey, { apdu }, ec);

	if (ec)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, ec.message().c_str());
		return;
	}

	/*
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "control key: ");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.controlKey, 17, 17);
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "monitor key: ");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.monitorKey, 17, 17);
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "");
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, plain text");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, apdu, 17, 17);
	FORMAT_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, hased text %u", hmac.Size());
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, hmac, 17, 17);
	*/

	sstate.sessions.SetSessionKeys(user, unwrapped.keys);

	auto rsp = this->StartResponse();
	rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
	rsp.SetControl(header.control);
	auto writer = rsp.GetWriter();
	auto hmacType = sstate.hmac.GetType();

	auto success = sstate.keyChangeState.FormatKeyStatusResponse(
		writer,
		user,
		hmacType,
		Crypto::ToKeyWrapAlgorithm(updateKeyType),
		KeyStatus::OK,
		hmac
		);

	if (success)
	{
		this->BeginTx(rsp.ToReadOnly());
	}
}

void OAuthContext::ProcessRequestKeyStatus(const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
{
	User user(status.userNum);
	UpdateKeyMode type;
	if (!sstate.pUserDatabase->GetUpdateKeyType(user, type))
	{
		// TODO  - the spec appears to just say "ignore users that don't exist". Confirm this.
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "User %u does not exist", user.GetId());
		return;
	}

	auto keyStatus = sstate.sessions.GetSessionKeyStatus(user);

	auto rsp = this->StartResponse();
	rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
	rsp.SetControl(header.control);
	auto writer = rsp.GetWriter();
	auto hmacType = HMACType::NO_MAC_VALUE;

	auto success = sstate.keyChangeState.FormatKeyStatusResponse(
		writer,
		user,
		hmacType,
		Crypto::ToKeyWrapAlgorithm(type),
		keyStatus,
		ReadBufferView::Empty()
		);

	if (!success)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to format key status response");
		return;
	}

	this->BeginTx(rsp.ToReadOnly());
}

void OAuthContext::ProcessAuthReply(const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
{
	// first look-up the session for the specified user
	User user(reply.userNum);
	SessionKeysView view;
	if (sstate.sessions.GetSessionKeys(user, view) != KeyStatus::OK)
	{
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED); // TODO - check this error code
		return;
	}

	if (!sstate.challenge.VerifyAuthenticity(view.controlKey, sstate.hmac, reply.hmacValue, this->logger))
	{
		// TODO  - log an auth failure
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	auto criticalHeader = sstate.challenge.GetCriticalHeader();

	if (sstate.pUserDatabase->IsAuthorized(user, criticalHeader.function))
	{
		auto asdu = sstate.challenge.GetCriticalASDU();
		auto objects = asdu.Skip(APDU_REQUEST_HEADER_SIZE);
		this->ProcessAPDU(asdu, criticalHeader, objects); // process as normal
	}
	else
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Verified user %u is not authorized for function %s", user.GetId(), FunctionCodeToString(criticalHeader.function));
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHORIZATION_FAILED);
	}
}

bool OAuthContext::TransmitChallenge(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header)
{
	auto response = this->StartResponse();
	auto success = sstate.challenge.WriteChallenge(apdu, header, response, sstate.hmac.GetType(), *sstate.pCrypto, &this->logger);
	if (success)
	{
		this->BeginTx(response.ToReadOnly());
	}
	return success;
}

openpal::IKeyWrapAlgo& OAuthContext::GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, UpdateKeyMode type)
{
	switch (type)
	{
	case(UpdateKeyMode::AES128) :
		return crypto.GetAES128KeyWrap();
	default:
		return crypto.GetAES256KeyWrap();
	}
}

void OAuthContext::RespondWithAuthError(
	const opendnp3::APDUHeader& header,	
	uint32_t seqNum,
	const User& user,
	AuthErrorCode code
	)
{
	auto rsp = this->StartResponse();
	rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
	rsp.SetControl(header.control);
	auto writer = rsp.GetWriter();

	Group120Var7 error;
	error.challengeSeqNum = seqNum;
	error.userNum = user.GetId();
	error.assocId = sstate.settings.assocId;
	error.errorCode = code;
	error.time = openpal::UInt48Type(sstate.pTimeSource->Now().msSinceEpoch);

	writer.WriteFreeFormat(error);

	this->BeginTx(rsp.ToReadOnly());
}

}



/*
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

#include "SessionKeyTask.h"

#include "KeyStatusHandler.h"
#include "KeyWrap.h"

#include "secauth/AuthSizes.h"

#include "secauth/Crypto.h"
#include "secauth/HMACProvider.h"

#include <opendnp3/app/parsing/APDUParser.h>

#include <openpal/crypto/SecureCompare.h>


using namespace opendnp3;
using namespace openpal;

namespace secauth
{

SessionKeyTask::SessionKeyTask(	opendnp3::IMasterApplication& application,
                                openpal::TimeDuration retryPeriod_,
                                openpal::TimeDuration changeInterval_,
                                openpal::Logger logger,
                                const User& user_,
                                ICryptoProvider& crypto,
                                IMasterUserDatabase& userDB,
                                SessionStore& sessionStore) :

	opendnp3::IMasterTask(application, openpal::MonotonicTimestamp::Min(), logger, TaskConfig::With(user_)),
	retryPeriod(retryPeriod_),
	changeInterval(changeInterval_),
	user(user_),
	pCrypto(&crypto),
	pUserDB(&userDB),
	pSessionStore(&sessionStore),
	state(ChangeState::GetStatus),
	keyChangeSeqNum(0)
{

}

bool SessionKeyTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	return (state == ChangeState::GetStatus) ? this->BuildStatusRequest(request, seq) : this->BuildSessionKeyRequest(request, seq);
}

void SessionKeyTask::Initialize()
{
	this->state = ChangeState::GetStatus;
}

IMasterTask::ResponseResult SessionKeyTask::ProcessResponse(const APDUResponseHeader& header, const openpal::RSlice& objects)
{
	if (ValidateSingleResponse(header))
	{
		return (state == ChangeState::GetStatus) ? OnStatusResponse(header, objects) : OnChangeResponse(header, objects);
	}
	else
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
}

IMasterTask::TaskState SessionKeyTask::OnTaskComplete(TaskCompletion result, openpal::MonotonicTimestamp now)
{
	switch (result)
	{
	case(TaskCompletion::SUCCESS) :
		return TaskState::Retry(now.Add(this->changeInterval));
	case(TaskCompletion::FAILURE_NO_COMMS) :
		return TaskState::Immediately();
	default:
		return TaskState::Retry(now.Add(this->retryPeriod));
	}
}

bool SessionKeyTask::BuildStatusRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	request.ConfigureHeader(FunctionCode::AUTH_REQUEST, seq);

	Group120Var4 ksrequest;
	ksrequest.userNum = user.GetId();

	return request.GetWriter().WriteSingleValue<UInt8, Group120Var4>(QualifierCode::UINT8_CNT, ksrequest);
}

bool SessionKeyTask::BuildSessionKeyRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	request.ConfigureHeader(FunctionCode::AUTH_REQUEST, seq);

	Group120Var6 sessionKeyChange;
	sessionKeyChange.keyChangeSeqNum = this->keyChangeSeqNum;
	sessionKeyChange.userNum = user.GetId();
	sessionKeyChange.keyWrapData = this->keyWrapBuffer.GetWrappedData();

	auto ret = request.GetWriter().WriteFreeFormat(sessionKeyChange);

	// save a view of what we're transmitting as we'll need it to validate the HMAC
	this->txKeyWrapASDU = request.ToRSlice();

	return ret;
}

IMasterTask::ResponseResult SessionKeyTask::OnStatusResponse(const APDUResponseHeader& response, const RSlice& objects)
{
	KeyStatusHandler handler(this->logger);
	if (IsFailure(APDUParser::Parse(objects, handler, this->logger)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	Group120Var5 status;
	RSlice rawObject;

	if (!handler.GetStatus(status, rawObject))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Response did not contain a key status object");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// save the KSQ
	this->keyChangeSeqNum = status.keyChangeSeqNum;

	if (!AuthSizes::ChallengeDataSizeWithinLimits(status.challengeData.Size()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Bad challenge data size");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// before we derive keys, make sure we support the specified key wrap algorithm
	if (status.keyWrapAlgo == KeyWrapAlgorithm::UNDEFINED)
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unsupported key wrap algorithm: %s", KeyWrapAlgorithmToString(status.keyWrapAlgo));
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	std::error_code ec;

	keys.DeriveFrom(*pCrypto, AuthSizes::MIN_SESSION_KEY_SIZE_BYTES, ec); // TODO - make the session key size configurable
	if (ec)
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unable to derive session keys: %s", ec.message().c_str());
		return ResponseResult::ERROR_BAD_RESPONSE; // TODO - add a different return code or is this good enough?
	}

	// get a view of the users update key
	auto key = this->pUserDB->GetUpdateKeyView(this->user);

	if (key.algorithm == KeyWrapAlgorithm::UNDEFINED)
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unable to get update key for user: %u", user.GetId());
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	if (status.keyWrapAlgo != key.algorithm)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Update key length does not match outstation KeyWrapAlgorithm");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	if (!this->keyWrapBuffer.Wrap(pCrypto->GetAESKeyWrap(), key.data, this->keys.GetView(), rawObject, this->logger))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to wrap session keys");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// The wrapped key data is now stored in the keyWrapBuffer until we can send it out

	this->state = ChangeState::ChangeKey; // we're now ready to try changing the key itself

	return ResponseResult::OK_REPEAT;
}

IMasterTask::ResponseResult SessionKeyTask::OnChangeResponse(const APDUResponseHeader& response, const RSlice& objects)
{
	KeyStatusHandler handler(this->logger);
	if (IsFailure(APDUParser::Parse(objects, handler, this->logger)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	Group120Var5 status;
	RSlice rawObject;

	if (!handler.GetStatus(status, rawObject))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Response did not contain a key status object");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	HMACMode hmacMode;
	if (!Crypto::TryGetHMACMode(status.hmacAlgo, hmacMode))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Outstation requested unsupported HMAC algorithm: %s", HMACTypeToString(status.hmacAlgo));
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	HMACProvider hmac(*pCrypto, hmacMode);
	std::error_code ec;
	auto hmacValue = hmac.Compute(keys.GetView().monitorKey, { this->txKeyWrapASDU }, ec);
	if (ec)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, ec.message().c_str());
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	if (!SecureEquals(hmacValue, status.hmacValue))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "Invalid HMAC received from outstation during session key swap");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// Set the session keys and return!
	this->pSessionStore->SetSessionKeys(user, keys.GetView());

	return ResponseResult::OK_FINAL;
}

}


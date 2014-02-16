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
#include "MockAppLayer.h"

#include <openpal/LoggableMacros.h>
#include <openpal/ToHex.h>
#include <openpal/Exception.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

MockAppLayer::MockAppLayer(openpal::Logger aLogger) :
	Loggable(aLogger),
	mNumCancel(0),
	mpUser(nullptr),
	mAutoSendCallback(true),
	mIsSuccess(true)
{

}

void MockAppLayer::SetUser(IAppUser* apUser)
{
	mpUser = apUser;
}

void MockAppLayer::EnableAutoSendCallback(bool aIsSuccess)
{
	mAutoSendCallback = true;
	mIsSuccess = aIsSuccess;
}

void MockAppLayer::DisableAutoSendCallback()
{
	mAutoSendCallback = false;
}

void MockAppLayer::DoSendUnsol()
{
	if(mAutoSendCallback) {
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnUnsolSendSuccess();
		else mpUser->OnUnsolFailure();
	}
}

void MockAppLayer::DoSendSol()
{
	if(mAutoSendCallback) {
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnSolSendSuccess();
		else mpUser->OnSolFailure();
	}
}

void MockAppLayer::SendResponse(APDUWrapper& apdu)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(apdu.ToReadOnly(), true));	
	mFragments.push_back(apdu);
	this->DoSendSol();

}

void MockAppLayer::SendUnsolicited(APDUWrapper& apdu)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(apdu.ToReadOnly(), true));	
	mFragments.push_back(apdu);
	this->DoSendUnsol();
}

void MockAppLayer::SendRequest(APDUWrapper& apdu)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(apdu.ToReadOnly(), true));	
	mFragments.push_back(apdu);
}

bool MockAppLayer::NothingToRead()
{
	return mFragments.size() == 0;
}

APDUWrapper MockAppLayer::Read()
{
	if(mFragments.size() == 0) throw InvalidStateException(LOCATION, "no more fragments");
	auto frag = mFragments.front();	
	mFragments.pop_front();
	return frag;
}

FunctionCode MockAppLayer::ReadFunction()
{
	if(mFragments.size() == 0) throw InvalidStateException(LOCATION, "No more fragments");
	else {
		FunctionCode func = mFragments.front().GetFunction();
		mFragments.pop_front();
		return func;
	}
}

void MockAppLayer::CancelResponse()
{
	++mNumCancel;
}

}



//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "LinkLayer.h"

#include <assert.h>

#include <opendnp3/Logger.h>
#include <opendnp3/Exception.h>
#include <opendnp3/DNPConstants.h>

#include "LoggableMacros.h"
#include "ILinkRouter.h"
#include "PriLinkLayerStates.h"
#include "SecLinkLayerStates.h"


namespace opendnp3
{

LinkLayer::LinkLayer(Logger* apLogger, IExecutor* apExecutor, const LinkConfig& arConfig) :
	Loggable(apLogger),
	ILowerLayer(apLogger),
	mCONFIG(arConfig),
	mRetryRemaining(0),
	mpExecutor(apExecutor),
	mpTimer(NULL),
	mNextReadFCB(false),
	mNextWriteFCB(false),
	mIsOnline(false),
	mpRouter(NULL),
	mpPriState(PLLS_SecNotReset::Inst()),
	mpSecState(SLLS_NotReset::Inst())
{}

void LinkLayer::SetRouter(ILinkRouter* apRouter)
{
	assert(mpRouter == NULL); assert(apRouter != NULL);
	mpRouter = apRouter;
}

void LinkLayer::ChangeState(PriStateBase* apState)
{
	mpPriState = apState;
}

void LinkLayer::ChangeState(SecStateBase* apState)
{
	mpSecState = apState;
}

bool LinkLayer::Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest)
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");

	if(aIsMaster == mCONFIG.IsMaster) {
		ERROR_BLOCK(LEV_WARNING,
		            (aIsMaster ? "Master frame received for master" : "Slave frame received for slave"),
		            DLERR_MASTER_BIT_MATCH);
		return false;
	}

	if(aDest != mCONFIG.LocalAddr) {
		ERROR_BLOCK(LEV_WARNING, "Frame for unknown destintation", DLERR_UNKNOWN_DESTINATION);
		return false;
	}

	if(aSrc != mCONFIG.RemoteAddr) {
		ERROR_BLOCK(LEV_WARNING, "Frame from unknwon source", DLERR_UNKNOWN_SOURCE);
		return false;
	}

	return true;
}

////////////////////////////////
// ILinkContext
////////////////////////////////

void LinkLayer::OnLowerLayerUp()
{
	if(mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerUp");
	mIsOnline = true;
	if(mpUpperLayer) mpUpperLayer->OnLowerLayerUp();
}

void LinkLayer::OnLowerLayerDown()
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");

	if(mpTimer != NULL) this->CancelTimer();
	mIsOnline = false;
	mpPriState = PLLS_SecNotReset::Inst();
	mpSecState = SLLS_NotReset::Inst();

	if(mpUpperLayer) mpUpperLayer->OnLowerLayerDown();
}

void LinkLayer::Transmit(const LinkFrame& arFrame)
{
	mpRouter->Transmit(arFrame);
}

void LinkLayer::SendAck()
{
	mSecFrame.FormatAck(mCONFIG.IsMaster, false, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mSecFrame);
}

void LinkLayer::SendLinkStatus()
{
	mSecFrame.FormatLinkStatus(mCONFIG.IsMaster, false, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mSecFrame);
}

void LinkLayer::SendResetLinks()
{
	mPriFrame.FormatResetLinkStates(mCONFIG.IsMaster, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mPriFrame);
}

void LinkLayer::SendUnconfirmedUserData(const uint8_t* apData, size_t aLength)
{
	mPriFrame.FormatUnconfirmedUserData(mCONFIG.IsMaster, mCONFIG.RemoteAddr, mCONFIG.LocalAddr, apData, aLength);
	this->Transmit(mPriFrame);
	this->DoSendSuccess();
}

void LinkLayer::SendDelayedUserData(bool aFCB)
{
	mDelayedPriFrame.ChangeFCB(aFCB);
	this->Transmit(mDelayedPriFrame);
}

void LinkLayer::StartTimer()
{
	assert(mpTimer == NULL);
	mpTimer = this->mpExecutor->Start(std::chrono::milliseconds(mCONFIG.Timeout), std::bind(&LinkLayer::OnTimeout, this));
}

void LinkLayer::CancelTimer()
{
	assert(mpTimer);
	mpTimer->Cancel();
	mpTimer = NULL;
}

void LinkLayer::ResetRetry()
{
	this->mRetryRemaining = mCONFIG.NumRetry;
}

bool LinkLayer::Retry()
{
	if(mRetryRemaining > 0) {
		--mRetryRemaining;
		return true;
	}
	else return false;
}

////////////////////////////////
// IFrameSink
////////////////////////////////

void LinkLayer::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->Ack(this, aIsRcvBuffFull);
}

void LinkLayer::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->Nack(this, aIsRcvBuffFull);
}

void LinkLayer::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->LinkStatus(this, aIsRcvBuffFull);
}

void LinkLayer::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->NotSupported(this, aIsRcvBuffFull);
}

void LinkLayer::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->TestLinkStatus(this, aFcb);
}

void LinkLayer::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->ResetLinkStates(this);
}

void LinkLayer::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->RequestLinkStatus(this);
}

void LinkLayer::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->ConfirmedUserData(this, aFcb, apData, aDataLength);
}

void LinkLayer::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->UnconfirmedUserData(this, apData, aDataLength);
}

////////////////////////////////
// ILowerLayer
////////////////////////////////

void LinkLayer::_Send(const uint8_t* apData, size_t aDataLength)
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");
	if(mCONFIG.UseConfirms) mpPriState->SendConfirmed(this, apData, aDataLength);
	else mpPriState->SendUnconfirmed(this, apData, aDataLength);
}

void LinkLayer::OnTimeout()
{
	assert(mpTimer);
	mpTimer = NULL;
	mpPriState->OnTimeout(this);
}

}


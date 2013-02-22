
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
#include "AppChannelStates.h"

#include <opendnp3/Singleton.h>
#include <opendnp3/Exception.h>
#include <opendnp3/Logger.h>

#include "LoggableMacros.h"
#include "AppLayerChannel.h"
#include "APDU.h"

#include <string>
#include <sstream>

namespace opendnp3
{

// ---- Default behaviors for the states ----

void ACS_Base::Send(AppLayerChannel*, APDU&, size_t)
{
	this->ThrowInvalidState(LOCATION);
}

void ACS_Base::Cancel(AppLayerChannel*)
{
	this->ThrowInvalidState(LOCATION);
}

void ACS_Base::OnSendSuccess(AppLayerChannel*)
{
	this->ThrowInvalidState(LOCATION);
}

void ACS_Base::OnSendFailure(AppLayerChannel*)
{
	this->ThrowInvalidState(LOCATION);
}

void ACS_Base::OnConfirm(AppLayerChannel* c, int aSeq)
{
	ERROR_LOGGER_BLOCK(c->GetLogger(), LEV_WARNING,
	                   "Unexpected confirm with sequence: " << aSeq, ALERR_UNEXPECTED_CONFIRM);
}

void ACS_Base::OnResponse(AppLayerChannel* c, APDU& arAPDU)
{
	LOGGER_BLOCK(c->GetLogger(), LEV_WARNING,
	             "Unexpected response with sequence: " << arAPDU.GetControl().SEQ);
}

void ACS_Base::OnTimeout(AppLayerChannel*)
{
	this->ThrowInvalidState(LOCATION);
}

void ACS_Base::ThrowInvalidState(const std::string& arLocation)
{
	MACRO_THROW_EXCEPTION_COMPLEX(InvalidStateException, "State: " << this->Name());
}

void ACS_Base::ProcessResponse(AppLayerChannel* c, APDU& arAPDU, bool aExpectFIR)
{
	AppControlField acf = arAPDU.GetControl();

	if(acf.SEQ == c->Sequence()) {
		if(acf.FIR == aExpectFIR) {
			c->CancelTimer();

			if(acf.FIN) {
				c->ChangeState(ACS_Idle::Inst());
				c->DoFinalResponse(arAPDU);
			}
			else {
				c->IncrSequence();
				c->ChangeState(ACS_WaitForFinalResponse::Inst());
				c->StartTimer();
				c->DoPartialResponse(arAPDU);
			}
		}
		else {
			ERROR_LOGGER_BLOCK(c->GetLogger(), LEV_WARNING, "Unexpected fir bit " << acf.FIR, ALERR_BAD_FIR_FIN);
		}
	}
	else {
		ERROR_LOGGER_BLOCK(c->GetLogger(), LEV_WARNING, "Bad sequence number " << acf.SEQ, ALERR_BAD_SEQUENCE);
	}
}

// ---- ACS_IDle ----

ACS_Idle ACS_Idle::mInstance;

void ACS_Idle::Send(AppLayerChannel* c, APDU& arAPDU, size_t aNumRetry)
{
	AppControlField acf = arAPDU.GetControl();
	FunctionCodes func = arAPDU.GetFunction();
	acf.SEQ = (acf.FIR && func == FC_RESPONSE) ? c->Sequence() : c->IncrSequence();
	arAPDU.SetControl(acf);
	c->ChangeState(NextState(c, arAPDU.GetFunction(), acf.CON));
	c->SetRetry(aNumRetry);
	c->QueueSend(arAPDU);
}

ACS_Base* ACS_Idle::NextState(AppLayerChannel* c, FunctionCodes aFunc, bool aConfirm)
{
	switch(aFunc) {
	case(FC_CONFIRM):
		MACRO_THROW_EXCEPTION(ArgumentException, "Confirms are automatic only");
	case(FC_RESPONSE):
		if(c->Sequence() < 0) {
			MACRO_THROW_EXCEPTION(InvalidStateException, "Can't respond until we've received a request");
		}

	case(FC_UNSOLICITED_RESPONSE):
		if(aConfirm) return ACS_SendConfirmed::Inst();
		else return ACS_Send::Inst();

	case(FC_DIRECT_OPERATE_NO_ACK):
		if(aConfirm) {
			MACRO_THROW_EXCEPTION(ArgumentException, "DO no ACK can't be confirmed");
		}
		return ACS_Send::Inst();

	default:	// it's a request with an expected response
		if(aConfirm) {
			MACRO_THROW_EXCEPTION(ArgumentException, "Confirmation not allowed for requests");
		}
		return ACS_SendExpectResponse::Inst();
	}
}

// ---- ACS_SendBase ----

void ACS_SendBase::OnSendFailure(AppLayerChannel* c)
{
	if(!c->Retry(this)) { //if we can't retry, then go back to idle
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

void ACS_SendBase::Cancel(AppLayerChannel* c)
{
	c->ChangeState(ACS_SendCanceled::Inst());
}

// ---- ACS_SendCanceled ----

ACS_SendCanceled ACS_SendCanceled::mInstance;

void ACS_SendCanceled::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoFailure();
}

void ACS_SendCanceled::OnSendFailure(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoFailure();
}

// ---- ACS_Send ----

ACS_Send ACS_Send::mInstance;

void ACS_Send::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoSendSuccess();
}

// ---- ACS_SendConfirmed ----

ACS_SendConfirmed ACS_SendConfirmed::mInstance;

void ACS_SendConfirmed::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_WaitForConfirm::Inst());
	c->StartTimer();
}

// ----  ACS_SendExpectResponse ----

ACS_SendExpectResponse  ACS_SendExpectResponse::mInstance;

void ACS_SendExpectResponse::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_WaitForFirstResponse::Inst());
	c->StartTimer();
}

// ---- ACS_WaitForConfirm ----

ACS_WaitForConfirm ACS_WaitForConfirm::mInstance;

void ACS_WaitForConfirm::Cancel(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->CancelTimer();
	c->DoFailure();
}

void ACS_WaitForConfirm::OnConfirm(AppLayerChannel* c, int aSeq)
{
	// does the confirm sequence match what we expect?
	if(c->Sequence() == aSeq) {
		c->CancelTimer();
		c->ChangeState(ACS_Idle::Inst());
		c->DoSendSuccess();
	}
	else {
		ERROR_LOGGER_BLOCK(c->GetLogger(), LEV_WARNING,
		                   "Unexpected confirm w/ sequence " << aSeq, ALERR_UNEXPECTED_CONFIRM);
	}

}

void ACS_WaitForConfirm::OnTimeout(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->GetLogger(), LEV_WARNING, "Timeout while waiting for confirm");
	if(!c->Retry(ACS_SendConfirmed::Inst())) {
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

// ---- ACS_WaitForResponseBase ----

void ACS_WaitForResponseBase::OnTimeout(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->GetLogger(), LEV_WARNING, "Timeout while waiting for response");
	if(!c->Retry(ACS_SendExpectResponse::Inst())) {
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

// ---- ACS_WaitForFirstResponse ----

ACS_WaitForFirstResponse ACS_WaitForFirstResponse::mInstance;

void ACS_WaitForFirstResponse::OnResponse(AppLayerChannel* c, APDU& arAPDU)
{
	this->ProcessResponse(c, arAPDU, true);
}

// ---- ATS_WaitForFinalResponse ----

ACS_WaitForFinalResponse ACS_WaitForFinalResponse::mInstance;

void ACS_WaitForFinalResponse::OnResponse(AppLayerChannel* c, APDU& arAPDU)
{
	this->ProcessResponse(c, arAPDU, false);
}

}


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

#include "OutstationContext.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/outstation/OutstationActions.h"
#include "opendnp3/outstation/OutstationFunctions.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

OContext::OContext(
	const OutstationConfig& config,
	const DatabaseTemplate& dbTemplate,
	openpal::Logger logger_,				
	openpal::IExecutor& executor,		
	ILowerLayer& lower,
	ICommandHandler& commandHandler,
	IOutstationApplication& application) :
	
		logger(logger_),
		pExecutor(&executor),
		pLower(&lower),	
		pCommandHandler(&commandHandler),
		pApplication(&application),
		eventBuffer(config.eventBufferConfig),
		database(dbTemplate, eventBuffer, config.params.indexMode, config.params.typesAllowedInClass0),
		rspContext(database.GetStaticLoader(), eventBuffer),
		params(config.params),	
		isOnline(false),
		isTransmitting(false),	
		staticIIN(IINBit::DEVICE_RESTART),	
		confirmTimer(executor),
		deferred(config.params.maxRxFragSize),
		sol(config.params.maxTxFragSize),
		unsol(config.params.maxTxFragSize)
{	
	
}

OutstationSolicitedStateBase* OContext::OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	// analyze this request to see how it compares to the last request
	if (this->history.HasLastRequest())
	{
		if (this->sol.seq.num.Equals(header.control.SEQ))
		{
			if (this->history.FullyEqualsLastRequest(header, objects))
			{
				if (header.function == FunctionCode::READ)
				{
					SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring repeat read request");
					return this->sol.pState;
				}
				else
				{
					return this->sol.pState->OnRepeatNonReadRequest(*this, header, objects);
				}
			}
			else // new operation with same SEQ
			{
				return this->ProcessNewRequest(header, objects);
			}
		}
		else  // completely new sequence #
		{
			return this->ProcessNewRequest(header, objects);
		}
	}
	else
	{
		return this->ProcessNewRequest(header, objects);
	}
}

OutstationSolicitedStateBase* OContext::ProcessNewRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->sol.seq.num = header.control.SEQ;

	if (header.function == FunctionCode::READ)
	{
		return this->sol.pState->OnNewReadRequest(*this, header, objects);
	}
	else
	{
		return this->sol.pState->OnNewNonReadRequest(*this, header, objects);
	}
}

OutstationSolicitedStateBase* OContext::RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->history.RecordLastProcessedRequest(header, objects);

	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
	auto iin = OFunctions::HandleNonReadResponse(*this, header, objects, writer);
	response.SetIIN(iin | this->GetResponseIIN());
	OActions::BeginResponseTx(*this, response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OContext::RespondToReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->history.RecordLastProcessedRequest(header, objects);

	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto result = OFunctions::HandleRead(*this, objects, writer);
	result.second.SEQ = header.control.SEQ;
	this->sol.seq.confirmNum = header.control.SEQ;
	response.SetControl(result.second);
	response.SetIIN(result.first | this->GetResponseIIN());
	OActions::BeginResponseTx(*this, response.ToReadOnly());

	if (result.second.CON)
	{
		OActions::StartSolicitedConfirmTimer(*this);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return  &OutstationSolicitedStateIdle::Inst();
	}
}

OutstationSolicitedStateBase* OContext::ContinueMultiFragResponse(const AppSeqNum& seq)
{
	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto control = this->rspContext.LoadResponse(writer);
	control.SEQ = seq;
	this->sol.seq.confirmNum = seq;
	response.SetControl(control);
	response.SetIIN(this->GetResponseIIN());
	OActions::BeginResponseTx(*this, response.ToReadOnly());

	if (control.CON)
	{
		OActions::StartSolicitedConfirmTimer(*this);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return &OutstationSolicitedStateIdle::Inst();
	}
}

bool OContext::GoOnline()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "already online");
		return false;
	}

	isOnline = true;
	this->CheckForTaskStart();	
	return true;
}

bool OContext::GoOffline()
{
	if (!isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "already offline");
		return false;
	}

	isOnline = false;
	isTransmitting = false;

	sol.Reset();
	unsol.Reset();
	history.Reset();
	deferred.Reset();
	eventBuffer.Unselect();
	rspContext.Reset();
	confirmTimer.Cancel();
	auth.Reset();

	return true;
}

bool OContext::CanTransmit() const
{
	return isOnline && !isTransmitting;
}

IINField OContext::GetResponseIIN()
{
	return this->staticIIN | this->GetDynamicIIN() | this->pApplication->GetApplicationIIN().ToIIN();
}

IINField OContext::GetDynamicIIN()
{
	auto classField = this->eventBuffer.UnwrittenClassField();

	IINField ret;
	ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
	ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
	ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
	ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, this->eventBuffer.IsOverflown());

	return ret;
}

void OContext::OnReceiveAPDU(const openpal::ReadBufferView& apdu)
{
	FORMAT_HEX_BLOCK(this->logger, flags::APP_HEX_RX, apdu, 18, 18);

	APDUHeader header;
	if (!APDUHeaderParser::ParseRequest(apdu, header, &this->logger))
	{		
		return;
	}


	FORMAT_LOG_BLOCK(this->logger, flags::APP_HEADER_RX,
		"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
		header.control.FIR,
		header.control.FIN,
		header.control.CON,
		header.control.UNS,
		header.control.SEQ,
		FunctionCodeToString(header.function));

	// outstations should only process single fragment messages that don't request confirmation
	if (!header.control.IsFirAndFin() || header.control.CON)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring fragment. Request must be FIR/FIN/!CON");
		return;
	}	

	auto objects = apdu.Skip(APDU_REQUEST_HEADER_SIZE);
	this->auth.OnReceive(*this, apdu, header, objects);
}

void OContext::OnSendResult(bool isSuccess)
{
	if (this->isTransmitting)
	{
		this->isTransmitting = false;
		this->CheckForTaskStart();		
	}
}

void OContext::CheckForTaskStart()
{
	// do these checks in order of priority
	this->auth.CheckState(*this);
	OActions::CheckForDeferredRequest(*this);
	OActions::CheckForUnsolicited(*this);
}


}


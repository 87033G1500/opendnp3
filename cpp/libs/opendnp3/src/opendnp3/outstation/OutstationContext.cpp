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

#include "opendnp3/app/APDULogging.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/APDUParser.h"
#include "opendnp3/app/APDUHeaderParser.h"

#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/WriteHandler.h"
#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"
#include "opendnp3/outstation/ConstantCommandAction.h"
#include "opendnp3/outstation/EventWriter.h"

#include "opendnp3/outstation/ClassBasedRequestHandler.h"
#include "opendnp3/outstation/AssignClassHandler.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

OutstationContext::OutstationContext(
		const OutstationConfig& config,
		const DatabaseTemplate& dbTemplate,
		openpal::IMutex* pMutex,
		IExecutor& executor,
		LogRoot& root,
		ILowerLayer& lower,
		ICommandHandler& commandHandler,
		IOutstationApplication& application,
		IOutstationAuthProvider& authProvider
		) :
	
		ostate(config, dbTemplate, pMutex, *this, executor, root, lower, commandHandler, application, authProvider)
{	
	
}

void OutstationContext::OnNewEventData()
{
	if (!ostate.pendingTaskCheckFlag)
	{
		ostate.pendingTaskCheckFlag = true;
		// post these calls so the stack can unwind
		auto lambda = [this]() 
		{ 
			this->ostate.pendingTaskCheckFlag = false;
			this->CheckForTaskStart(); 
		};
		ostate.pExecutor->PostLambda(lambda);
	}
}

IINField OutstationContext::GetDynamicIIN()
{	
	auto classField = ostate.eventBuffer.UnwrittenClassField();

	IINField ret;
	ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
	ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
	ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
	ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, ostate.eventBuffer.IsOverflown());
	
	return ret;
}

IINField OutstationContext::GetResponseIIN()
{
	return this->ostate.staticIIN | GetDynamicIIN() | ostate.pApplication->GetApplicationIIN().ToIIN();
}

void OutstationContext::ConfigureUnsolHeader(APDUResponse& unsol)
{	
	build::NullUnsolicited(unsol, this->ostate.unsol.seqN, this->GetResponseIIN());
}

void OutstationContext::SetOnline()
{
	ostate.isOnline = true;
	this->CheckForTaskStart();
}

void OutstationContext::SetOffline()
{
	ostate.Reset();		
}


void OutstationContext::OnReceiveAPDU(const openpal::ReadBufferView& apdu)
{	
	APDUHeader header;	
	if (APDUHeaderParser::ParseRequest(apdu, header, &ostate.logger))
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::APP_HEADER_RX,
			"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
			header.control.FIR,
			header.control.FIN,
			header.control.CON,
			header.control.UNS,
			header.control.SEQ,
			FunctionCodeToString(header.function));

		// outstations should only process single fragment messages that don't request confirmation
		if (header.control.IsFirAndFin() && !header.control.CON)
		{
			auto objects = apdu.Skip(APDU_REQUEST_HEADER_SIZE);
			ostate.pAuthProvider->ExamineASDU(*this, header, objects);
		}
		else
		{
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring fragment. Requst must be FIR/FIN/!CON");
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(ostate.logger, flags::ERR, "ignoring malformed request header");
	}


	// regardless of what the event is, see if we need to schedule an action
	this->CheckForTaskStart();
}

void OutstationContext::OnSendResult(bool isSuccess)
{
	if (ostate.isOnline && ostate.isTransmitting)
	{
		ostate.isTransmitting = false;
		this->CheckForTaskStart();		
	}	
}

void OutstationContext::ExamineASDU(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.unsol.pState = ostate.unsol.pState->OnConfirm(this, header);
		}
		else
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring unsol with invalid function code: %s", FunctionCodeToString(header.function));
		}
	}
	else
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.sol.pState = ostate.sol.pState->OnConfirm(this, header);
		}
		else
		{
			ostate.sol.pState = this->OnReceiveSolRequest(header, objects);
			ostate.history.RecordLastRequest(header, objects);
		}
	}
}

OutstationSolicitedStateBase* OutstationContext::OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{			
	// analyze this request to see how it compares to the last request
	if (ostate.history.HasLastRequest())
	{		
		if (this->ostate.sol.seqN == header.control.SEQ)
		{			
			if (ostate.history.FullyEqualsLastRequest(header, objects))
			{
				if (header.function == FunctionCode::READ)
				{
					SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring repeat read request");
					return ostate.sol.pState;
				}
				else
				{
					return this->ostate.sol.pState->OnRepeatNonReadRequest(this, header, objects);
				}
			}
			else // new operation with same SEQ
			{
				return ProcessNewRequest(header, objects);
			}
		}
		else  // completely new sequence #
		{
			return ProcessNewRequest(header, objects);
		}		
	}
	else
	{		
		return ProcessNewRequest(header, objects);
	}	
}

OutstationSolicitedStateBase* OutstationContext::ProcessNewRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->ostate.sol.seqN = header.control.SEQ;
	if (header.function == FunctionCode::READ)
	{
		return this->ostate.sol.pState->OnNewReadRequest(this, header, objects);
	}
	else
	{
		return this->ostate.sol.pState->OnNewNonReadRequest(this, header, objects);
	}
}

OutstationSolicitedStateBase* OutstationContext::RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
		auto iin = this->BuildNonReadResponse(header, objects, writer);
		response.SetIIN(iin | this->GetResponseIIN());
	};

	auto response = ostate.txBuffers.FormatSolicited(format);		
	this->BeginResponseTx(response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OutstationContext::RespondToReadRequest(uint8_t seq, const openpal::ReadBufferView& objects)
{
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		auto result = this->HandleRead(objects, writer);
		result.second.SEQ = seq;
		ostate.sol.expectedConSeqN = seq;
		response.SetControl(result.second);
		response.SetIIN(result.first | this->GetResponseIIN());
	};

	auto response = ostate.txBuffers.FormatSolicited(format);
	this->BeginResponseTx(response.ToReadOnly());	
	if (response.GetControl().CON)
	{
		this->StartSolicitedConfirmTimer();
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return  &OutstationSolicitedStateIdle::Inst();
	}	
}

void OutstationContext::ProcessNoResponseFunction(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
		case(FunctionCode::DIRECT_OPERATE_NR) :
			this->HandleDirectOperate(objects, nullptr); // no object writer, this is a no ack code
			break;
		default:
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring NR function code: %s", FunctionCodeToString(header.function));
			break;
	}	
}

void OutstationContext::BeginResponseTx(const ReadBufferView& response)
{		
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	this->ostate.isTransmitting = true;	
	ostate.pLower->BeginTransmit(response);
}

void OutstationContext::BeginUnsolTx(const ReadBufferView& response)
{	
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	this->ostate.isTransmitting = true;
	this->ostate.unsol.expectedConSeqN = ostate.unsol.seqN;
	this->ostate.unsol.seqN = AppControlField::NextSeq(ostate.unsol.seqN);
	ostate.pLower->BeginTransmit(response);
}

IINField OutstationContext::BuildNonReadResponse(const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	switch (header.function)
	{				
		case(FunctionCode::WRITE) :
			return HandleWrite(objects);
		case(FunctionCode::SELECT) :
			return HandleSelect(objects, writer);
		case(FunctionCode::OPERATE) :
			return HandleOperate(objects, writer);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(objects, &writer);
		case(FunctionCode::COLD_RESTART) :
			return HandleRestart(objects, false, &writer);
		case(FunctionCode::WARM_RESTART) :
			return HandleRestart(objects, true, &writer);
		case(FunctionCode::ASSIGN_CLASS) :
			return HandleAssignClass(objects);
		case(FunctionCode::DELAY_MEASURE) :
			return HandleDelayMeasure(objects, writer);
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return ostate.params.allowUnsolicited ? HandleDisableUnsolicited(objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return ostate.params.allowUnsolicited ? HandleEnableUnsolicited(objects, writer) : IINField(IINBit::FUNC_NOT_SUPPORTED);
		default:
			return	IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

OutstationSolicitedStateBase* OutstationContext::ContinueMultiFragResponse(uint8_t seq)
{	
	auto format = [&](APDUResponse& response) {
		auto writer = response.GetWriter();
		response.SetFunction(FunctionCode::RESPONSE);
		auto control = this->ostate.rspContext.LoadResponse(writer);
		control.SEQ = seq;
		ostate.sol.expectedConSeqN = seq;
		response.SetControl(control);
		response.SetIIN(this->ostate.staticIIN | this->GetDynamicIIN());
	};

	auto response = ostate.txBuffers.FormatSolicited(format);
	this->BeginResponseTx(response.ToReadOnly());
	
	if (response.GetControl().CON)
	{
		this->StartSolicitedConfirmTimer();
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return &OutstationSolicitedStateIdle::Inst();
	}	
}

void OutstationContext::CheckForTaskStart()
{		
	// if we're online, the solicited state is idle, and the unsolicited state 
	// is not transmitting we may be able to do a task
	if (ostate.isOnline && !ostate.isTransmitting && ostate.sol.IsIdle())
	{		
		if (ostate.deferred.IsSet())
		{			
			if (ostate.deferred.GetFunction() == FunctionCode::READ)
			{
				if (ostate.unsol.IsIdle())
				{					
					auto handler = [this](const APDUHeader& header, ReadBufferView objects, bool)
					{
						ostate.sol.pState = ostate.sol.pState->OnNewReadRequest(this, header, objects);
					};
					ostate.deferred.Process(handler);
				}
			}
			else
			{
				auto handler = [this](const APDUHeader& header, ReadBufferView objects, bool isRepeat)
				{
					if (isRepeat)
					{
						ostate.sol.pState = ostate.sol.pState->OnRepeatNonReadRequest(this, header, objects);
					}
					else
					{
						ostate.sol.pState = ostate.sol.pState->OnNewNonReadRequest(this, header, objects);
					}
				};

				ostate.deferred.Process(handler);
			}
		}
		else
		{		
			if (ostate.unsol.IsIdle())
			{
				this->CheckForUnsolicited();
			}
		}
	}	
}

void OutstationContext::CheckForUnsolicited()
{
	if (ostate.params.allowUnsolicited)
	{
		if (ostate.unsol.completedNull)
		{				
			// are there events to be reported?
			if (ostate.params.unsolClassMask.Intersects(ostate.eventBuffer.UnwrittenClassField()))
			{			
				auto format = [this](APDUResponse& response) 
				{
					auto writer = response.GetWriter();
					// even though we're not loading static data, we need to lock 
					// the database since it updates the event buffer
					Transaction tx(ostate.database);
					ostate.eventBuffer.Unselect();
					ostate.eventBuffer.SelectAllByClass(ostate.params.unsolClassMask);
					ostate.eventBuffer.Load(writer);
					this->ConfigureUnsolHeader(response);
				};
			
				auto response = ostate.txBuffers.FormatUnsolicited(format);								
				this->StartUnsolicitedConfirmTimer();
				this->ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				this->BeginUnsolTx(response.ToReadOnly());								
			}			
		}
		else
		{
			// send a NULL unsolcited message			
			auto format = [this](APDUResponse& response) 
			{
				this->ConfigureUnsolHeader(response);
			};
			
			auto response = ostate.txBuffers.FormatUnsolicited(format);
			this->StartUnsolicitedConfirmTimer();
			this->ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
			this->BeginUnsolTx(response.ToReadOnly());
		}
	}	
}

bool OutstationContext::StartSolicitedConfirmTimer()
{
	auto timeout = [this]() 
	{ 
		ostate.sol.pState = this->ostate.sol.pState->OnConfirmTimeout(this);
		this->CheckForTaskStart();
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

bool OutstationContext::StartUnsolicitedConfirmTimer()
{
	auto timeout = [this]() 
	{ 
		ostate.unsol.pState = this->ostate.unsol.pState->OnConfirmTimeout(this);
		this->CheckForTaskStart();
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

Pair<IINField, AppControlField> OutstationContext::HandleRead(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ostate.rspContext.Reset();

	// Do a transaction (lock) on the database  for multi-threaded environments
	Transaction tx(ostate.database);
	ostate.eventBuffer.Unselect(); // always unselect any perviously selected points when we start a new read request
	ostate.database.Unselect();
	ReadHandler handler(ostate.logger, ostate.database.GetSelector(), ostate.eventBuffer);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger, APDUParser::Settings::NoContents()); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{				
		auto control = ostate.rspContext.LoadResponse(writer);
		return Pair<IINField, AppControlField>(handler.Errors(), control);
	}
	else
	{
		ostate.rspContext.Reset();
		return Pair<IINField, AppControlField>(IINFromParseResult(result), AppControlField(true, true, false, false));
	}
}

IINField OutstationContext::HandleWrite(const openpal::ReadBufferView& objects)
{
	WriteHandler handler(ostate.logger, *ostate.pApplication, &ostate.staticIIN);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OutstationContext::HandleDirectOperate(const openpal::ReadBufferView& objects, HeaderWriter* pWriter)
{
	// since we're echoing, make sure there's enough size before beginning
	if (pWriter && (objects.Size() > pWriter->Remaining()))
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %u", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ostate.pCommandHandler, false);
		CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, pWriter);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
		return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleSelect(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(ostate.pCommandHandler, true);
		CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, &writer);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
		if (result == APDUParser::Result::OK)
		{
			if (handler.AllCommandsSuccessful())
			{						
				ostate.control.Select(ostate.sol.seqN, ostate.pExecutor->GetTime(), objects);				
			}
			
			return handler.Errors();
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField OutstationContext::HandleOperate(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Igonring command request due to oversized payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		auto now = ostate.pExecutor->GetTime();
		auto result = ostate.control.ValidateSelection(ostate.sol.seqN, now, ostate.params.selectTimeout, objects);

		if (result == CommandStatus::SUCCESS)
		{
			CommandActionAdapter adapter(ostate.pCommandHandler, false);
			CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &adapter, &writer);
			auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
			return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
		}
		else
		{
			return HandleCommandWithConstant(objects, writer, result);
		}
	}
}

IINField OutstationContext::HandleDelayMeasure(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	if (objects.IsEmpty())
	{		
		Group52Var2 value = { 0 }; 	// respond with 0 time delay
		writer.WriteSingleValue<UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);
		return IINField::Empty();
	}
	else
	{
		// there shouldn't be any trailing headers in delay measure request, no need to even parse
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField OutstationContext::HandleRestart(const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter)
{
	if (objects.IsEmpty())
	{
		auto mode = isWarmRestart ? ostate.pApplication->WarmRestartSupport() : ostate.pApplication->ColdRestartSupport();

		switch (mode)
		{
			case(RestartMode::UNSUPPORTED) :
				return IINField(IINBit::FUNC_NOT_SUPPORTED);
			case(RestartMode::SUPPORTED_DELAY_COARSE) :
			{
				auto delay = isWarmRestart ? ostate.pApplication->WarmRestart() : ostate.pApplication->ColdRestart();
				if (pWriter)
				{
					Group52Var1 coarse = { delay };
					pWriter->WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, coarse);
				}
				return IINField::Empty();
			}
			default:
			{
				auto delay = isWarmRestart ? ostate.pApplication->WarmRestart() : ostate.pApplication->ColdRestart();
				if (pWriter)
				{
					Group52Var2 fine = { delay };
					pWriter->WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, fine);
				}
				return IINField::Empty();
			}
		}		
	}
	else
	{
		// there shouldn't be any trailing headers in restart requests, no need to even parse
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField OutstationContext::HandleAssignClass(const openpal::ReadBufferView& objects)
{
	if (ostate.pApplication->SupportsAssignClass())
	{		
		AssignClassHandler handler(ostate.logger, *ostate.pExecutor, *ostate.pApplication, ostate.database.GetClassAssigner());

		// Lock the db as this can adjust configuration values in the database
		Transaction tx(ostate.database);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger, APDUParser::Settings::NoContents());
		return (result == APDUParser::Result::OK) ? handler.Errors() : IINFromParseResult(result);
	}
	else
	{
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField OutstationContext::HandleDisableUnsolicited(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	if (result == APDUParser::Result::OK)
	{
		ostate.params.unsolClassMask.Clear(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleEnableUnsolicited(const openpal::ReadBufferView& objects, HeaderWriter& writer)
{	
	ClassBasedRequestHandler handler(ostate.logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	if (result == APDUParser::Result::OK)
	{
		ostate.params.unsolClassMask.Set(handler.GetClassField());
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleCommandWithConstant(const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(ostate.logger, ostate.params.maxControlsPerRequest, &constant, &writer);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &ostate.logger);
	return IINFromParseResult(result);
}


}


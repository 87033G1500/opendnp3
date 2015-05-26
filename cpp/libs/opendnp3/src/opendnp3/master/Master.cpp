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

#include "Master.h"

#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/LogLevels.h"

#include "opendnp3/master/UserPollTask.h"
#include "opendnp3/master/WriteTask.h"
#include "opendnp3/master/MeasurementHandler.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

Master::Master(
	openpal::IExecutor& executor,
	openpal::LogRoot& root,
	ILowerLayer& lower,
	ISOEHandler& SOEHandler,	
	opendnp3::IMasterApplication& application,
	const MasterParams& params,
	ITaskLock& taskLock
	) : 
	mstate(executor, root, lower, SOEHandler, application, *this, params, taskLock),
	commandProcessor(mstate)
{}
	
void Master::OnLowerLayerUp()
{
	mstate.GoOnline();
}

void Master::OnLowerLayerDown()
{
	mstate.GoOffline();
}

void Master::OnReceive(const openpal::ReadBufferView& apdu)
{
	if (mstate.isOnline)
	{
		APDUResponseHeader header;
		if (APDUHeaderParser::ParseResponse(apdu, header, &mstate.logger))
		{
			FORMAT_LOG_BLOCK(mstate.logger, flags::APP_HEADER_RX,
				"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
				header.control.FIR,
				header.control.FIN,
				header.control.CON,
				header.control.UNS,
				header.control.SEQ,
				FunctionCodeToString(header.function),
				header.IIN.LSB,
				header.IIN.MSB);

			if (header.control.UNS)
			{
				if (header.function == FunctionCode::UNSOLICITED_RESPONSE)
				{
					this->OnUnsolicitedResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported function with UNS bit set: %s", FunctionCodeToString(header.function));
				}
			}
			else
			{
				if (header.function == FunctionCode::RESPONSE)
				{					
					mstate.OnResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported solicited function code: %s", FunctionCodeToString(header.function));
				}
			}
		}
	}
}

void Master::OnUnsolicitedResponse(const APDUResponseHeader& header, const ReadBufferView& objects)
{		
	auto result = MeasurementHandler::ProcessMeasurements(objects, mstate.logger, mstate.pSOEHandler);

	if ((result == ParseResult::OK) && header.control.CON)
	{
		mstate.QueueConfirm(APDUHeader::UnsolicitedConfirm(header.control.SEQ));
	}	
}

void Master::OnReceiveIIN(const IINField& iin)
{	
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{
		mstate.tasks.clearRestart.Demand();
		mstate.tasks.assignClass.Demand();
		mstate.tasks.startupIntegrity.Demand();
		mstate.tasks.enableUnsol.Demand();
	}

	if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && mstate.params.integrityOnEventOverflowIIN)
	{
		mstate.tasks.startupIntegrity.Demand();
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{
		mstate.tasks.timeSync.Demand();
	}

	if ((iin.IsSet(IINBit::CLASS1_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass1()) ||
		(iin.IsSet(IINBit::CLASS2_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass2()) ||
		(iin.IsSet(IINBit::CLASS3_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass3()))
	{
		mstate.tasks.eventScan.Demand();
	}

	mstate.pApplication->OnReceiveIIN(iin);
}

void Master::OnPendingTask()
{	
	mstate.PostCheckForTask();
}

void Master::OnSendResult(bool isSucccess)
{
	if (mstate.isOnline && mstate.isSending)
	{
		mstate.isSending = false;

		mstate.CheckConfirmTransmit();		
		mstate.CheckForTask();
	}
}

ICommandProcessor& Master::GetCommandProcessor()
{
	return commandProcessor;
}

void Master::SetAuthProvider(IMasterAuthProvider& provider)
{
	this->mstate.auth.SetProvider(provider);
}

MasterScan Master::AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback, int userId)
{
	auto pTask = new UserPollTask(builder, true, period, mstate.params.taskRetryPeriod, *mstate.pApplication, *mstate.pSOEHandler, pCallback, userId, mstate.logger);
	this->ScheduleRecurringPollTask(pTask);	
	auto callback = [this]() { this->mstate.PostCheckForTask(); };
	return MasterScan(*mstate.pExecutor, pTask, callback);
}

MasterScan Master::AddClassScan(const ClassField& field, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{	
	auto configure = [field](HeaderWriter& writer) 
	{ 
		build::WriteClassHeaders(writer, field); 
	};
	return this->AddScan(period, configure, pCallback, userId);
}

MasterScan Master::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId](HeaderWriter& writer)
	{ 
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);		
	};
	return this->AddScan(period, configure, pCallback, userId);
}

MasterScan Master::AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);		
	};
	return this->AddScan(period, configure, pCallback, userId);
}

void Master::Scan(const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback, int userId)
{
	auto pTask = new UserPollTask(builder, false, TimeDuration::Max(), mstate.params.taskRetryPeriod, *mstate.pApplication, *mstate.pSOEHandler, pCallback, userId, mstate.logger);
	this->ScheduleAdhocTask(pTask);	
}

void Master::ScanClasses(const ClassField& field, ITaskCallback* pCallback, int userId)
{
	auto configure = [field](HeaderWriter& writer)
	{
		build::WriteClassHeaders(writer, field);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::ScanAllObjects(GroupVariationID gvId, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId](HeaderWriter& writer)
	{
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::Write(const TimeAndInterval& value, uint16_t index, ITaskCallback* pCallback, int userId)
{
	auto format = [value, index](HeaderWriter& writer)
	{
		writer.WriteSingleIndexedValue<UInt16, TimeAndInterval>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group50Var4::Inst(), value, index);
	};

	auto pTask = new WriteTask(*mstate.pApplication, format, mstate.logger, pCallback, userId);
	this->ScheduleAdhocTask(pTask);
}

/// ------ private helpers ----------

void Master::ScheduleRecurringPollTask(IMasterTask* pTask)
{
	mstate.tasks.BindTask(pTask);

	if (mstate.isOnline)
	{
		mstate.scheduler.Schedule(ManagedPtr<IMasterTask>::WrapperOnly(pTask));
		mstate.PostCheckForTask();		
	}
}

void Master::ScheduleAdhocTask(IMasterTask* pTask)
{
	auto task = ManagedPtr<IMasterTask>::Deleted(pTask);
	if (mstate.isOnline)
	{
		mstate.scheduler.Schedule(std::move(task));
		mstate.PostCheckForTask();		
	}
	else
	{
		// can't run this task since we're offline so fail it immediately
		pTask->OnLowerLayerClose(mstate.pExecutor->GetTime());
	}
}
	
}

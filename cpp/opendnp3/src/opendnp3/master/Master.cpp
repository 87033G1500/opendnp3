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

#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/LogLevels.h"


#include <openpal/LogMacros.h>

namespace opendnp3
{

Master::Master(
	openpal::IExecutor& executor,
	openpal::LogRoot& root,
	openpal::ILowerLayer& lower,
	ISOEHandler* pSOEHandler,
	const MasterParams& params
	) : 
	context(executor, root, lower, pSOEHandler, params),
	commandMarshaller(executor, context)
{}
	
void Master::OnLowerLayerUp()
{
	context.OnLayerUp();
}

void Master::OnLowerLayerDown()
{
	context.OnLayerDown();
}

void Master::OnReceive(const openpal::ReadOnlyBuffer& apdu)
{
	if (context.isOnline)
	{
		APDUResponseRecord response;
		auto result = APDUHeaderParser::ParseResponse(apdu, response, &context.logger);
		if (result == APDUHeaderParser::Result::OK)
		{			
			FORMAT_LOG_BLOCK(context.logger, flags::APP_HEADER_RX,
				"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
				response.control.FIN,
				response.control.FIN,
				response.control.CON,
				response.control.UNS,
				response.control.SEQ,
				FunctionCodeToString(response.function),
				response.IIN.LSB,
				response.IIN.MSB);

			switch (response.function)
			{
				case(FunctionCode::RESPONSE) :
					context.OnResponse(response);
					break;
				case(FunctionCode::UNSOLICITED_RESPONSE) :
					context.OnUnsolicitedResponse(response);
					break;
				default:
					FORMAT_LOG_BLOCK(context.logger, flags::WARN, "unsupported function code: %s", FunctionCodeToString(response.function));
					break;
			}
		}
	}
}

void Master::OnSendResult(bool isSucccess)
{
	if (context.isOnline)
	{
		context.OnSendResult(isSucccess);
	}
}

ICommandProcessor& Master::GetCommandProcessor()
{
	return commandMarshaller;
}

MasterScan Master::AddScan(openpal::TimeDuration period, const openpal::Function1<APDURequest&> builder)
{
	PollTask task(builder, period, context.pSOEHandler, &context.logger);
	auto pTask = context.scheduler.AddPollTask(task);
	if (pTask)
	{
		return MasterScan(*context.pExecutor, context.scheduler, *pTask);
	}
	else
	{
		return MasterScan();
	}
}

MasterScan Master::AddClassScan(int classMask, openpal::TimeDuration period)
{	
	auto configure = [classMask](APDURequest& request) { build::WriteClassHeaders(request, classMask); };
	return this->AddScan(period, openpal::Bind1<APDURequest&>(configure));
}
	
}

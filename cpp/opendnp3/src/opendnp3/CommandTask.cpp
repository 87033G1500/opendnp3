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
#include "CommandTask.h"

#include <openpal/Exception.h>

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

CommandTask::CommandTask(Logger aLogger) : MasterTaskBase(aLogger)
{

}

void CommandTask::Configure(const Formatter& arFormatter, const Responder& arResponder)
{
	mFormatter = arFormatter;
	mResponder = arResponder;
	mCodes.clear();
}

void CommandTask::AddCommandCode(FunctionCode aCode)
{
	this->mCodes.push_back(aCode);
}

void CommandTask::ConfigureRequest(APDU& arAPDU)
{
	if(mCodes.empty()) MACRO_THROW_EXCEPTION(InvalidStateException, "No more functions in sequence");
	mValidator = mFormatter(arAPDU, mCodes.front());
	mCodes.pop_front();
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string CommandTask::Name() const
{
	return "CommandTask";
}
#endif

void CommandTask::OnFailure()
{
	mResponder(CommandResponse(CommandResult::TIMEOUT));
}

TaskResult CommandTask::_OnPartialResponse(const APDUResponseRecord&)
{
	LOG_BLOCK(LogLevel::Error, "Non fin responses not allowed for control tasks");
	return TR_CONTINUE;
}

TaskResult CommandTask::_OnFinalResponse(const APDUResponseRecord&)
{
	CommandStatus cs = CommandStatus::FORMAT_ERROR; // TODO - mValidator(arAPDU); TODO - move validation to something simplier

	if(cs == CommandStatus::SUCCESS) {
		if(mCodes.empty()) {
			mResponder(CommandResponse(CommandResult::RESPONSE_OK, cs));
			return TR_SUCCESS;
		}
		else return TR_CONTINUE;
	}
	else {
		mResponder(CommandResponse(CommandResult::RESPONSE_OK, cs));
		return TR_SUCCESS;
	}
}

} //ens ns



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

#include "MasterContext.h"

#include "opendnp3/LogLevels.h"

namespace opendnp3
{

MasterContext::MasterContext(
	openpal::IExecutor& executor_,
	openpal::LogRoot& root,
	openpal::ILowerLayer& lower_,
	ISOEHandler* pSOEHandler,
	const MasterParams& params_
	) :

	logger(root.GetLogger(sources::MASTER)),
	executor(executor_),
	lower(lower_),
	params(params_),
	isOnline(false),
	isSending(false),
	solSeq(0),
	unsolSeq(0),
	pActiveTask(nullptr),
	scheduler(executor_),
	taskList(pSOEHandler, &logger, params)
{}


void MasterContext::OnResponse(const APDUResponseRecord& response)
{
	
}

void MasterContext::OnUnsolicitedResponse(const APDUResponseRecord& response)
{

}

}

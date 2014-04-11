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

using namespace openpal;

namespace opendnp3
{

OutstationContext::OutstationContext(
		IExecutor& executor,
		LogRoot& root,
		ILowerLayer& lower,
		ICommandHandler& commandHandler,
		Database& database,
		EventBufferFacade& buffers) :
	
	logger(root.GetLogger()),	
	pExecutor(&executor),
	pLower(&lower),
	pCommandHandler(&commandHandler),
	pDatabase(&database),
	eventBuffer(buffers),
	isOnline(false),
	isSending(false),
	firstValidRequestAccepted(false),
	rxFragCount(0),		
	operateExpectedSeq(0),
	operateExpectedFragCount(0),
	solSeqN(0),
	expectedConfirmSeq(0),
	unsolSeq(0),
	rspContext(&database, &eventBuffer, StaticResponseTypes())	
{
	
}

void OutstationContext::SetOnline()
{
	isOnline = true;
}

void OutstationContext::SetOffline()
{
	isOnline = false;
	isSending = false;
	firstValidRequestAccepted = false;
}

void OutstationContext::Select()
{
	operateExpectedFragCount = rxFragCount + 1;
	operateExpectedSeq = NextSeq(solSeqN);
	selectTime = pExecutor->GetTime();
}

bool OutstationContext::IsOperateValid()
{	
	return (rxFragCount == operateExpectedFragCount) && (solSeqN == operateExpectedSeq);	
}

}


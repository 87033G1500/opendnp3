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
#include "TransportStates.h"

#include "TransportLayer.h"

namespace opendnp3
{

//////////////////////////////////////////////////////
//	TLS_Ready
//////////////////////////////////////////////////////
TLS_Ready TLS_Ready::mInstance;

void TLS_Ready::Send(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Sending::Inst());
	apContext->TransmitAPDU(arBuffer);
}

void TLS_Ready::HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(arBuffer);
}

//////////////////////////////////////////////////////
//	TLS_Sending
//////////////////////////////////////////////////////
TLS_Sending TLS_Sending::mInstance;

void TLS_Sending::HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer* apContext)
{
	apContext->ReceiveTPDU(arBuffer);
}

void TLS_Sending::HandleSendSuccess(TransportLayer* apContext)
{
	if(!apContext->ContinueSend())
	{
		apContext->ChangeState(TLS_Ready::Inst()); // order important here
		apContext->SignalSendResult(true);
	}
}

void TLS_Sending::HandleSendFailure(TransportLayer* apContext)
{
	apContext->ChangeState(TLS_Ready::Inst());
	apContext->SignalSendResult(false);
}

} //end namespace


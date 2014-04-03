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
#ifndef __APP_CHANNEL_STATES_H_
#define __APP_CHANNEL_STATES_H_

#include "opendnp3/Singleton.h"

#include "opendnp3/gen/FunctionCode.h"
#include "opendnp3/app/AppLayerChannel.h"
#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUWrapper.h"

namespace opendnp3
{


/** Defines interface that
	concrete states must implement/override - AppChannelState(ACS)_Base
*/
class ACS_Base
{
public:

	virtual void Send(AppLayerChannel*, APDUWrapper&, uint32_t aNumRetry);
	virtual void Cancel(AppLayerChannel*); //cancel the outbound transaction

	// external events
	virtual void OnSendSuccess(AppLayerChannel*);
	virtual void OnSendFailure(AppLayerChannel*);
	virtual void OnConfirm(AppLayerChannel*, uint8_t aSequence);
	virtual void OnResponse(AppLayerChannel*, const APDUResponseRecord&);
	virtual void OnTimeout(AppLayerChannel*);

	virtual char const* Name() const = 0;

	virtual bool AcceptsResponse()
	{
		return false;
	}

protected:

	void ProcessResponse(AppLayerChannel*, const APDUResponseRecord&, bool aExpectFIR);

};

class ACS_Idle : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Idle)
	void Send(AppLayerChannel*, APDUWrapper&, uint32_t aNumRetry);

private:
	ACS_Base* NextState(AppLayerChannel* c, FunctionCode, bool aConfirm);
};

//default implementations for failure and cancel events
class ACS_SendBase : public ACS_Base
{
public:

	void OnSendFailure(AppLayerChannel*) override final;
	void Cancel(AppLayerChannel* c) override final;
};

class ACS_Send : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Send)

	void OnSendSuccess(AppLayerChannel* c) override final;
};

class ACS_SendConfirmed : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_SendConfirmed)

	void OnSendSuccess(AppLayerChannel*) override final;
};

class ACS_SendExpectResponse : public ACS_SendBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendExpectResponse);

	void OnSendSuccess(AppLayerChannel*) override final;

};

class ACS_SendCanceled : public ACS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendCanceled);

	void Cancel(AppLayerChannel*)  override final {} //do nothing if we're canceled and we cancel again
	void OnSendSuccess(AppLayerChannel*) override final;
	void OnSendFailure(AppLayerChannel*) override final;
};

class ACS_WaitForConfirm : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_WaitForConfirm)

	void Cancel(AppLayerChannel*) override final;
	void OnConfirm(AppLayerChannel*, uint8_t sequence) override final;
	void OnTimeout(AppLayerChannel*) override final;
};

class ACS_WaitForResponseBase : public ACS_Base
{
public:
	void OnTimeout(AppLayerChannel*) override final;
	bool AcceptsResponse() override final
	{
		return true;
	}
};



class ACS_WaitForFirstResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFirstResponse);

	void OnResponse(AppLayerChannel*, const APDUResponseRecord&);
};

class ACS_WaitForFinalResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFinalResponse);

	void OnResponse(AppLayerChannel*, const APDUResponseRecord&);
};

}

#endif


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
#include "PhysicalLayerWrapper.h"

#include <openpal/LoggableMacros.h>

#include <opendnp3/LogLevels.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

PhysicalLayerWrapper::PhysicalLayerWrapper(openpal::Logger logger, IPhysicalLayerAsync* apProxy) :
	Loggable(logger),	
	mCorruptionProbability(-1.0),
	mpProxy(apProxy),
	mpHandler(nullptr)
{
	mpProxy->SetHandler(this);
}

void PhysicalLayerWrapper::SetHandler(IHandlerAsync* apHandler)
{
	assert(apHandler != nullptr);
	assert(mpHandler == nullptr);
	mpHandler = apHandler;
}

void PhysicalLayerWrapper::AsyncOpen()
{
	return mpProxy->AsyncOpen();
}

void PhysicalLayerWrapper::AsyncClose()
{
	return mpProxy->AsyncClose();
}

void PhysicalLayerWrapper::AsyncWrite(const openpal::ReadOnlyBuffer&  arBuffer)
{
	return mpProxy->AsyncWrite(arBuffer);
}

void PhysicalLayerWrapper::AsyncRead(WriteBuffer& arBuffer)
{
	return mpProxy->AsyncRead(arBuffer);
}

void PhysicalLayerWrapper::OnLowerLayerUp()
{
	if (mpHandler)
	{
		mpHandler->OnLowerLayerUp();
	}
}

void PhysicalLayerWrapper::OnLowerLayerDown()
{
	if (mpHandler)
	{
		mpHandler->OnLowerLayerDown();
	}
}

void PhysicalLayerWrapper::OnReceive(const openpal::ReadOnlyBuffer& buffer)
{
	if(mCorruptionProbability > mRandom.Next())
	{
		LOG_BLOCK(flags::INFO, "Corrupting data");
		uint8_t* pData = const_cast<uint8_t*>(buffer.operator const uint8_t * ());		
		for(size_t i = 0; i < buffer.Size(); ++i) pData[i] = 0;
	}

	if (mpHandler)
	{
		mpHandler->OnReceive(buffer);
	}
}

void PhysicalLayerWrapper::OnSendResult(bool isSuccess)
{
	if (mpHandler)
	{
		mpHandler->OnSendResult(isSuccess);
	}
}

void PhysicalLayerWrapper::OnOpenFailure()
{
	if (mpHandler)
	{
		mpHandler->OnOpenFailure();
	}
}

void PhysicalLayerWrapper::SetCorruptionProbability(double aProbability)
{
	mCorruptionProbability = aProbability;
}

}

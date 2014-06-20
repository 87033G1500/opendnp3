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

#include "StackActionHandler.h"

#include <openpal/executor/IExecutor.h>
#include <openpal/executor/Bind.h>

#include "opendnp3/link/LinkLayerRouter.h"

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

StackActionHandler::StackActionHandler(LinkLayerRouter* pRouter_, openpal::IExecutor* pExecutor_, openpal::ITypedShutdownHandler<IStack*>* pHandler_) :
	pRouter(pRouter_),
	pExecutor(pExecutor_),
	pHandler(pHandler_)
{

}

openpal::IExecutor* StackActionHandler::GetExecutor()
{
	return pExecutor;
}

void StackActionHandler::EnableRoute(ILinkContext* pContext)
{
	auto lambda = [=]() { pRouter->Enable(pContext); };
	pExecutor->Post(Bind(lambda));
}

void StackActionHandler::DisableRoute(ILinkContext* pContext)
{
	auto lambda = [=]() { pRouter->Disable(pContext); };
	pExecutor->Post(Bind(lambda));
}

void StackActionHandler::BeginShutdown(ILinkContext* pContext, IStack* pStack)
{
	{
		// The pause has no affect if it's running on the executor
		openpal::ExecutorPause pause(pExecutor);
		pRouter->Remove(pContext);
	}

	auto lambda = [this, pStack]() { pHandler->OnShutdown(pStack); };
	pExecutor->Post(Bind(lambda));
}

}



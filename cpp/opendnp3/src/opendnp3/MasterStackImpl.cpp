
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "MasterStackImpl.h"

#include <openpal/IExecutor.h>

using namespace openpal;

namespace opendnp3
{

MasterStackImpl::MasterStackImpl(	Logger& arLogger,
                                        IExecutor* apExecutor,
                                        IDataObserver* apPublisher,
                                        IUTCTimeSource* apTimeSource,
                                        AsyncTaskGroup* apTaskGroup,
                                        const MasterStackConfig& arCfg,
                                        std::function<void (IMaster*)> aOnShutdown) :

	IMaster(arLogger),
	mpExecutor(apExecutor),
	mAppStack(arLogger, apExecutor, arCfg.app, arCfg.link),
	mMaster(arLogger.GetSubLogger("master"), arCfg.master, &mAppStack.mApplication, apPublisher, apTaskGroup, apExecutor, apTimeSource),
	mOnShutdown(aOnShutdown)
{
	mAppStack.mApplication.SetUser(&mMaster);
}

ICommandProcessor* MasterStackImpl::GetCommandProcessor()
{
	return mMaster.GetCommandProcessor();
}

ILinkContext* MasterStackImpl::GetLinkContext()
{
	return &mAppStack.mLink;
}

void MasterStackImpl::SetLinkRouter(ILinkRouter* apRouter)
{
	mAppStack.mLink.SetRouter(apRouter);
}

void MasterStackImpl::Shutdown()
{
	mOnShutdown(this);
}

void MasterStackImpl::AddStateListener(std::function<void (StackState)> aListener)
{
	mMaster.AddStateListener(aListener);
}

}


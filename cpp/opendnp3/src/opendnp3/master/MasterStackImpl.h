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
#ifndef __MASTER_STACK_IMPL_H_
#define __MASTER_STACK_IMPL_H_

#include "opendnp3/master/IMaster.h"
#include "opendnp3/master/MasterStackConfig.h"
#include "opendnp3/master/Master.h"

#include "opendnp3/app/ApplicationStack.h"


namespace opendnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class MasterStackImpl : public IMaster
{
public:

	MasterStackImpl(
	    openpal::Logger,
	    openpal::IExecutor* apExecutor,
	    ISOEHandler* apPublisher,
	    IUTCTimeSource* apTimeSource,
	    AsyncTaskGroup* apTaskGroup,
	    const MasterStackConfig& arCfg,
	    std::function<void (bool)> aEnableDisableFunc,
	    std::function<void (IMaster*)> aOnShutdown);

	ICommandProcessor* GetCommandProcessor();

	ILinkContext* GetLinkContext();

	void SetLinkRouter(ILinkRouter* apRouter);

	void AddStateListener(std::function<void (StackState)> aListener);

	MasterScan GetIntegrityScan();

	openpal::IExecutor* GetExecutor()
	{
		return mpExecutor;
	}

	MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate);

	void Shutdown();

private:
	IExecutor* mpExecutor;
	ApplicationStack mAppStack;
	Master mMaster;
	std::function<void (IMaster*)> mOnShutdown;

};

}

#endif


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
#ifndef __OUTSTATION_STACK_IMPL_H_
#define __OUTSTATION_STACK_IMPL_H_

#include "opendnp3/outstation/IOutstation.h"
#include "opendnp3/outstation/SlaveStackConfig.h"
#include "opendnp3/outstation/DatabaseConfiguration.h"
#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/Outstation.h"

#include "opendnp3/outstation/DynamicallyAllocatedDatabase.h"
#include "opendnp3/outstation/DynamicallyAllocatedEventBuffer.h"

#include <openpal/Location.h>

#include "opendnp3/app/ApplicationStack.h"
#include "opendnp3/Mutex.h"

namespace opendnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class OutstationStackImpl : public IOutstation
{
public:

	OutstationStackImpl(
		openpal::Logger&,
		openpal::IExecutor* apExecutor,
		ITimeWriteHandler* apTimeWriteHandler,
		ICommandHandler* apCmdHandler,
		const SlaveStackConfig& arCfg,
		const StackActionHandler& handler);

	IMeasurementLoader* GetLoader();

	void SetNeedTimeIIN();	

private:
	openpal::IExecutor* pExecutor;	
	DynamicallyAllocatedDatabase databaseBuffers;
	DynamicallyAllocatedEventBuffer eventBuffers;
	Mutex mutex;
	Database database;
	Outstation outstation;
};

}

#endif


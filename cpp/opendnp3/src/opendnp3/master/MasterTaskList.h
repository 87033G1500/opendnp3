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

#ifndef __MASTER_TASK_LIST_H_
#define __MASTER_TASK_LIST_H_

#include "opendnp3/master/EnableUnsolicitedTask.h"
#include "opendnp3/master/DisableUnsolicitedTask.h"
#include "opendnp3/master/StartupIntegrityPoll.h"
#include "opendnp3/master/ITaskList.h"

#include "opendnp3/StaticSizeConfiguration.h"

#include <openpal/StaticQueue.h>

namespace opendnp3
{

class MasterTaskList : public ITaskList
{
	
public:

	MasterTaskList(ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_, const MasterParams& params);

	void Initialize();

	virtual void ScheduleNext(IMasterScheduler& scheduler) override final;	
	
private:

	const MasterParams* pParams;
	EnableUnsolicitedTask enableUnsol;
	DisableUnsolicitedTask disableUnsol;	
	StartupIntegrityPoll startupIntegrity;

	openpal::StaticQueue<IMasterTask*, uint8_t, 8> startupTasks;	
};

}



#endif

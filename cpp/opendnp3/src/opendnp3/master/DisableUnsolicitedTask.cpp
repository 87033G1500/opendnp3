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

#include "DisableUnsolicitedTask.h"

#include "opendnp3/app/APDUBuilders.h"


namespace opendnp3
{

DisableUnsolicitedTask::DisableUnsolicitedTask(ITaskList* pTaskList_, openpal::Logger* pLogger_) :
	SingleResponseTask(pLogger_),
	pTaskList(pTaskList_)
{

}

void DisableUnsolicitedTask::BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq)
{
	build::DisableUnsolicited(request, seq);
}

void DisableUnsolicitedTask::OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler)
{
	scheduler.ScheduleLater(this, params.taskRetryPeriod);	
}

TaskStatus DisableUnsolicitedTask::OnSingleResponse(const APDUResponseRecord& response, const MasterParams& params, IMasterScheduler& scheduler)
{
	pTaskList->ScheduleNext(scheduler);
	return TaskStatus::SUCCESS;
}


} //end ns


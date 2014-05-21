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

#include "ClearRestartTask.h"

#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>

namespace opendnp3
{

ClearRestartTask::ClearRestartTask(openpal::Logger* pLogger_) : 
	SingleResponseTask(pLogger_),
	failed(false)
{

}	

void ClearRestartTask::BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq)
{
	build::ClearRestartIIN(request, seq);
}

void ClearRestartTask::Reset()
{
	failed = false;
}

bool ClearRestartTask::IsFailed() const
{
	return failed;
}

void ClearRestartTask::OnTimeoutOrBadControlOctet(const MasterParams& params, IMasterScheduler& scheduler)
{
	// timeout or bad control octet
	scheduler.ScheduleLater(this, params.taskRetryPeriod);
}
	
TaskStatus ClearRestartTask::OnSingleResponse(const APDUResponseRecord& response, const MasterParams& params, IMasterScheduler& scheduler)
{
	if (response.IIN.IsSet(IINBit::DEVICE_RESTART))
	{
		failed = true;

		// we tried to clear the restart, but the device responded with the restart still set
		SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, "Clear restart task failed to clear restart bit");		
		return TaskStatus::FAIL;
	}
	else
	{
		return TaskStatus::SUCCESS;
	}
}

} //end ns


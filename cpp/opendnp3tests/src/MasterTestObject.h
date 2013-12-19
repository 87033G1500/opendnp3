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
#ifndef __MASTER_TEST_OBJECT_H_
#define __MASTER_TEST_OBJECT_H_

#include "MockExecutor.h"
#include "LogTester.h"

#include <opendnp3/Master.h>
#include <opendnp3/AsyncTaskGroup.h>

#include <deque>

#include "MockMeasurementHandler.h"
#include "MockAppLayer.h"

namespace opendnp3
{


struct MasterConfig;

class MasterTestObject
{
public:
	MasterTestObject(MasterConfig, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	void RespondToMaster(const std::string& arData, bool aFinal = true);
	void SendUnsolToMaster(const std::string& arData);
	std::string Read();

	void BindStateListener();

	LogTester log;
	openpal::FixedUTCTimeSource fixedUTC;
	MockExecutor mts;
	AsyncTaskGroup group;
	MockMeasurementHandler meas;
	MockAppLayer app;
	Master master;
	//APDU mAPDU;
	std::deque<StackState> states;
};

}

#endif


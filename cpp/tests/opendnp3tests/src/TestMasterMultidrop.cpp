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
#include <catch.hpp>

#include "MasterTestObject.h"
#include "HexConversions.h"
#include "APDUHexBuilders.h"

#include <asiodnp3/MultidropTaskLock.h>

using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;

#define SUITE(name) "MasterMultidropTestSuite - " name

TEST_CASE(SUITE("MultidropRoundRobinStartupSequence"))
{
	MultidropTaskLock taskLock;

	MasterParams params;
	params.disableUnsolOnStartup = false;

	MasterTestObject t1(params, taskLock);
	MasterTestObject t2(params, taskLock);

	t1.master.OnLowerLayerUp();
	t2.master.OnLowerLayerUp();

	t1.exe.RunMany();
	t2.exe.RunMany();

	REQUIRE(t1.lower.PopWriteAsHex() == hex::IntegrityPoll(0));
	REQUIRE(t2.lower.PopWriteAsHex() == "");

	t1.master.OnSendResult(true);
	t1.SendToMaster(hex::EmptyResponse(0, IINField(IINBit::DEVICE_RESTART)));

	t1.exe.RunMany();
	t2.exe.RunMany();

	REQUIRE(t1.lower.PopWriteAsHex() == "");
	REQUIRE(t2.lower.PopWriteAsHex() == hex::IntegrityPoll(0));

	t2.master.OnSendResult(true);
	t2.SendToMaster(hex::EmptyResponse(0));

	t1.exe.RunMany();
	t2.exe.RunMany();

	REQUIRE(t1.lower.PopWriteAsHex() == hex::ClearRestartIIN(1));
	REQUIRE(t2.lower.PopWriteAsHex() == "");

}


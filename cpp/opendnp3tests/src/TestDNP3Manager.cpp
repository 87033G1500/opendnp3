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

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/IChannel.h>

#include <opendnp3/outstation/IOutstation.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>

#include <opendnp3/master/IMaster.h>
#include <opendnp3/master/ISOEHandler.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>


#include <asiopal/UTCTimeSource.h>





#include <thread>

using namespace opendnp3;
using namespace asiopal;

#define SUITE(name) "DNP3ManagerTestSuite - " name

const size_t ITERATIONS = 100;

TEST_CASE(SUITE("ConstructionDestruction"))
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(Logger(&log, LogLevel::Info, "server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(Logger(&log, LogLevel::Info, "serverChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		pClient->AddMaster("master", LogLevel::Info, NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		pServer->AddOutstation("outstation", LogLevel::Info, SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig(DatabaseTemplate()));
	}
}

TEST_CASE(SUITE("ManualStackShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(Logger(&log, LogLevel::Info, "server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(Logger(&log, LogLevel::Info, "serverChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", LogLevel::Info, SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig(DatabaseTemplate()));
		auto pMaster = pClient->AddMaster("master", LogLevel::Info, NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());

		pOutstation->Shutdown();
		pMaster->Shutdown();
	}

}

TEST_CASE(SUITE("ManualChannelShutdownWithStack"))
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pChannel = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys);

		pChannel->AddMaster("master", LogLevel::Info, NullSOEHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		pChannel->Shutdown();
	}
}

TEST_CASE(SUITE("ManualChannelShutdown"))
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pClientPhys)->Shutdown();

	}
}






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
#include "DNP3ManagerAdapter.h"

#include "Conversions.h"
#include "LogAdapter.h"
#include "ChannelAdapter.h"

#include <asiodnp3/ASIODNP3Manager.h>

using namespace asiopal;

namespace DNP3
{
namespace Adapter
{
IDNP3Manager ^ DNP3ManagerFactory::CreateManager(System::Int32 aConcurrency)
{
	return gcnew DNP3ManagerAdapter(aConcurrency);
}

IDNP3Manager ^ DNP3ManagerFactory::CreateManager()
{
	return gcnew DNP3ManagerAdapter(Environment::ProcessorCount);
}


DNP3ManagerAdapter::DNP3ManagerAdapter(System::Int32 aConcurrency) :
	mpMgr(new asiodnp3::ASIODNP3Manager(aConcurrency))
{

}

DNP3ManagerAdapter::~DNP3ManagerAdapter()
{
	delete mpMgr;
}

IChannel ^ DNP3ManagerAdapter::AddTCPClient(System::String ^ name, DNP3::Interface::LogLevel level, System::TimeSpan retryDelay, System::String ^ address, System::UInt16 port)
{

	std::string stdName = Conversions::convertString(name);
	std::string stdAddress = Conversions::convertString(address);
	uint16_t stdPort = port;
	auto lev = Conversions::convertLogLevel(level);

	try {
		Logger logger(mpMgr->GetLog(), lev, stdName);
		auto pChannel = mpMgr->AddTCPClient(logger, TimeDuration::Milliseconds(retryDelay.Milliseconds), stdAddress, stdPort);
		return gcnew ChannelAdapter(pChannel);
	}
	catch(openpal::Exception ex) {
		throw Conversions::convertException(ex);
	}
}

IChannel ^ DNP3ManagerAdapter::AddTCPServer(System::String ^ name, DNP3::Interface::LogLevel level, System::TimeSpan retryDelay, System::String ^ endpoint, System::UInt16 port)
{
	std::string stdName = Conversions::convertString(name);
	std::string stdEndpoint = Conversions::convertString(endpoint);
	uint16_t stdPort = port;
	auto lev = Conversions::convertLogLevel(level);

	try {
		Logger logger(mpMgr->GetLog(), lev, stdName);
		auto pChannel = mpMgr->AddTCPServer(logger, TimeDuration::Milliseconds(retryDelay.Milliseconds), stdEndpoint, stdPort);
		return gcnew ChannelAdapter(pChannel);
	}
	catch(openpal::Exception ex) {
		throw Conversions::convertException(ex);
	}
}

IChannel ^ DNP3ManagerAdapter::AddSerial(System::String ^ name, DNP3::Interface::LogLevel level, System::TimeSpan retryDelay, DNP3::Interface::SerialSettings ^ settings)
{
	std::string stdName = Conversions::convertString(name);
	auto lev = Conversions::convertLogLevel(level);
	auto s = Conversions::convertSerialSettings(settings);

	try {
		Logger logger(mpMgr->GetLog(), lev, stdName);
		auto pChannel = mpMgr->AddSerial(logger, TimeDuration::Milliseconds(retryDelay.Milliseconds), s);
		return gcnew ChannelAdapter(pChannel);
	}
	catch(openpal::Exception ex) {
		throw Conversions::convertException(ex);
	}
}

void DNP3ManagerAdapter::AddLogHandler(ILogHandler ^ logHandler)
{
	try {
		LogAdapterWrapper ^ wrapper = gcnew LogAdapterWrapper(logHandler);
		mpMgr->AddLogSubscriber(wrapper->GetLogAdapter());
	}
	catch(openpal::Exception ex) {
		throw Conversions::convertException(ex);
	}
}
}
}

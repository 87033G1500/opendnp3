
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "DNP3Manager.h"

#include "Conversions.h"
#include "LogAdapter.h"
#include "ChannelAdapter.h"
#include <DNP3/DNP3Manager.h>

namespace DNP3
{	
namespace Adapter
{

	DNP3Manager::DNP3Manager(System::UInt32 aConcurrency) :
		pMgr(new apl::dnp::DNP3Manager(aConcurrency))
	{
		
	}

	DNP3Manager::~DNP3Manager()
	{
		delete pMgr;
	}

	IChannel^ DNP3Manager::AddTCPClient(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ address, System::UInt16 port)
	{
		
		std::string stdName = Conversions::convertString(name);
		std::string stdAddress = Conversions::convertString(address);
		uint16_t stdPort = port;
		auto lev = Conversions::convertFilterLevel(level);
		
		try {
			auto pChannel = pMgr->AddTCPClient(stdName, lev, retryMs, stdAddress, stdPort);
			return gcnew ChannelAdapter(pChannel);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	IChannel^ DNP3Manager::AddTCPServer(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ endpoint, System::UInt16 port)
	{
		std::string stdName = Conversions::convertString(name);
		std::string stdEndpoint = Conversions::convertString(endpoint);		
		uint16_t stdPort = port;
		auto lev = Conversions::convertFilterLevel(level);		
		
		try {
			auto pChannel = pMgr->AddTCPServer(stdName, lev, retryMs, stdEndpoint, stdPort);
			return gcnew ChannelAdapter(pChannel);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	IChannel^ DNP3Manager::AddSerial(System::String^ name, FilterLevel level, System::UInt64 retryMs, SerialSettings^ settings)
	{
		std::string stdName = Conversions::convertString(name);
		auto lev = Conversions::convertFilterLevel(level);		
		auto s = Conversions::convertSerialSettings(settings);
		
		try {
			auto pChannel = pMgr->AddSerial(stdName, lev, retryMs, s);
			return gcnew ChannelAdapter(pChannel);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	void DNP3Manager::AddLogHandler(ILogHandler^ logHandler)
	{
		try {
			LogAdapterWrapper^ wrapper = gcnew LogAdapterWrapper(logHandler);
			pMgr->AddLogSubscriber(wrapper->GetLogAdapter());
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}
}}

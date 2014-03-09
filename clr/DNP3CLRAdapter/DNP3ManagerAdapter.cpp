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
IDNP3Manager^ DNP3ManagerFactory::CreateManager(System::Int32 aConcurrency)
{
	return gcnew DNP3ManagerAdapter(aConcurrency);
}

IDNP3Manager^ DNP3ManagerFactory::CreateManager()
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

IChannel^ DNP3ManagerAdapter::AddTCPClient(System::String^ name, DNP3::Interface::LogLevel level, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ address, System::UInt16 port)
{

	std::string stdName = Conversions::convertString(name);
	std::string stdAddress = Conversions::convertString(address);
	uint16_t stdPort = port;
	auto lev = Conversions::convertLogLevel(level);


	Logger logger(mpMgr->GetLog(), lev, stdName);
	auto pChannel = mpMgr->AddTCPClient(logger, Conversions::convertTimeSpan(minRetryDelay), Conversions::convertTimeSpan(maxRetryDelay), stdAddress, stdPort);
	if (pChannel == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew ChannelAdapter(pChannel);
	}
}

IChannel^ DNP3ManagerAdapter::AddTCPServer(System::String^ name, DNP3::Interface::LogLevel level, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ endpoint, System::UInt16 port)
{
	std::string stdName = Conversions::convertString(name);
	std::string stdEndpoint = Conversions::convertString(endpoint);
	uint16_t stdPort = port;
	auto lev = Conversions::convertLogLevel(level);
	Logger logger(mpMgr->GetLog(), lev, stdName);
	auto pChannel = mpMgr->AddTCPServer(logger, Conversions::convertTimeSpan(minRetryDelay), Conversions::convertTimeSpan(maxRetryDelay), stdEndpoint, stdPort);
	if (pChannel == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew ChannelAdapter(pChannel);
	}
}

IChannel^ DNP3ManagerAdapter::AddSerial(System::String^ name, DNP3::Interface::LogLevel level, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, DNP3::Interface::SerialSettings^ settings)
{
	std::string stdName = Conversions::convertString(name);
	auto lev = Conversions::convertLogLevel(level);
	auto s = Conversions::convertSerialSettings(settings);

	Logger logger(mpMgr->GetLog(), lev, stdName);
	auto pChannel = mpMgr->AddSerial(logger, Conversions::convertTimeSpan(minRetryDelay), Conversions::convertTimeSpan(maxRetryDelay), s);
	return gcnew ChannelAdapter(pChannel);
	if (pChannel == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew ChannelAdapter(pChannel);
	}
}

void DNP3ManagerAdapter::AddLogHandler(ILogHandler^ logHandler)
{
	LogAdapterWrapper^ wrapper = gcnew LogAdapterWrapper(logHandler);
	mpMgr->AddLogSubscriber(wrapper->GetLogAdapter());
}

}
}

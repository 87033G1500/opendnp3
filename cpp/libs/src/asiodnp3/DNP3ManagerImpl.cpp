/*
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

#include "DNP3ManagerImpl.h"

#include <opendnp3/LogLevels.h>

#ifdef OPENDNP3_USE_TLS
#include "asiodnp3/tls/MasterTLSServer.h"
#endif

#include "asiodnp3/ErrorCodes.h"
#include "asiodnp3/IOHandler.h"
#include "asiodnp3/DNP3Channel.h"
#include "asiodnp3/MasterTCPServer.h"

#include "asiopal/TCPClientChannelFactory.h"
#include "asiopal/TCPServerChannelFactory.h"

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

DNP3ManagerImpl::DNP3ManagerImpl(
    uint32_t concurrencyHint,
    std::shared_ptr<openpal::ILogHandler> handler,
    std::function<void()> onThreadStart,
    std::function<void()> onThreadExit
) :
	logger(handler, "manager", opendnp3::levels::ALL),
	io(std::make_shared<asiopal::IO>()),
	threadpool(logger, io, opendnp3::flags::INFO, concurrencyHint, onThreadStart, onThreadExit),
	resources(ResourceManager::Create())
{}

DNP3ManagerImpl::~DNP3ManagerImpl()
{
	this->Shutdown();
}

void DNP3ManagerImpl::Shutdown()
{
	if (resources)
	{
		resources->Shutdown();
		resources.reset();
	}
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTCPClient(
    const std::string& id,
    uint32_t levels,
    const ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    std::shared_ptr<IChannelListener> listener)
{
	auto create = [&]()
	{
		auto clogger = this->logger.Detach(id, levels);
		auto executor = StrandExecutor::Create(this->io);
		auto factory = TCPClientChannelFactory::Create(clogger, executor, retry, IPEndpoint(host, port), local);
		auto iohandler = IOHandler::Create(clogger, factory, listener);
		return DNP3Channel::Create(clogger, executor, iohandler, this->resources);
	};

	return this->resources->Bind<IChannel>(create);
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTCPServer(
    const std::string& id,
    uint32_t levels,
    const ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port,
    std::shared_ptr<IChannelListener> listener)
{
	auto create = [&]()
	{
		std::error_code ec;
		auto clogger = this->logger.Detach(id, levels);
		auto executor = StrandExecutor::Create(this->io);
		auto factory = TCPServerChannelFactory::Create(clogger, executor, IPEndpoint(endpoint, port), ec);
		auto iohandler = IOHandler::Create(clogger, factory, listener);
		return ec ? nullptr : DNP3Channel::Create(clogger, executor, iohandler, this->resources);
	};

	return this->resources->Bind<IChannel>(create);
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddSerial(
    const std::string& id,
    uint32_t levels,
    const ChannelRetry& retry,
    SerialSettings settings,
    std::shared_ptr<IChannelListener> listener)
{
	//ec = Error::NO_SERIAL_SUPPORT;
	return nullptr;
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTLSClient(
    const std::string& id,
    uint32_t levels,
    const ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    const TLSConfig& config,
    std::shared_ptr<IChannelListener> listener,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#else
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#endif

}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTLSServer(
    const std::string& id,
    uint32_t levels,
    const ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port,
    const TLSConfig& config,
    std::shared_ptr<IChannelListener> listener,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#else
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#endif

}

std::shared_ptr<asiopal::IListener> DNP3ManagerImpl::CreateListener(
	std::string loggerid,
	openpal::LogFilters levels,
	asiopal::IPEndpoint endpoint,
	const std::shared_ptr<IListenCallbacks>& callbacks,
	std::error_code& ec)
{
	auto create = [&]() -> std::shared_ptr<asiopal::IListener>
	{
		return asiodnp3::MasterTCPServer::Create(
			this->logger.Detach(loggerid, levels),
			asiopal::StrandExecutor::Create(this->io),
			endpoint,
			callbacks,
			this->resources,
			ec
		);
	};

	auto listener = this->resources->Bind<asiopal::IListener>(create);

	if (!listener)
	{
		ec = Error::SHUTTING_DOWN;
	}

	return listener;
}

std::shared_ptr<asiopal::IListener> DNP3ManagerImpl::CreateListener(
	std::string loggerid,
	openpal::LogFilters levels,
	asiopal::IPEndpoint endpoint,
	const asiopal::TLSConfig& config,
	const std::shared_ptr<IListenCallbacks>& callbacks,
	std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS

	auto create = [&]() -> std::shared_ptr<asiopal::IListener>
	{
		return asiodnp3::MasterTLSServer::Create(
			this->logger.Detach(loggerid, levels),
			asiopal::StrandExecutor::Create(this->io),
			endpoint,
			config,
			callbacks,
			this->resources,
			ec
		);
	};

	auto listener = this->resources->Bind<asiopal::IListener>(create);

	if (!listener)
	{
		ec = Error::SHUTTING_DOWN;
	}

	return listener;

#else

	ec = Error::NO_TLS_SUPPORT;
	return nullptr;

#endif

}

}


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

#include "asiopal/TCPServer.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include <sstream>

using namespace asio;
using namespace asio::ip;
using namespace opendnp3;

namespace asiopal
{

TCPServer::TCPServer(
    std::shared_ptr<StrandExecutor> executor,
    std::shared_ptr<ITCPServerHandler> handler,
	const openpal::Logger& logger,
    IPEndpoint endpoint,
    std::error_code& ec) :
	executor(executor),
	handler(handler),
	logger(logger),
	endpoint(ip::tcp::v4(), endpoint.port),
	acceptor(executor->strand.get_io_service()),
	socket(executor->strand.get_io_service()),
	session_id(0)
{
	this->Configure(endpoint.address, ec);
}

void TCPServer::BeginShutdown()
{
	acceptor.close();
}

void TCPServer::Configure(const std::string& adapter, std::error_code& ec)
{
	auto address = asio::ip::address::from_string(adapter, ec);

	if (ec)
	{
		return;
	}

	endpoint.address(address);
	acceptor.open(this->endpoint.protocol(), ec);

	if (ec)
	{
		return;
	}

	acceptor.set_option(ip::tcp::acceptor::reuse_address(true), ec);

	if (ec)
	{
		return;
	}

	acceptor.bind(this->endpoint, ec);

	if (ec)
	{
		return;
	}

	acceptor.listen(socket_base::max_connections, ec);

	if (!ec)
	{
		std::ostringstream oss;
		oss << this->endpoint;
		FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Listening on: %s", oss.str().c_str());
	}
}

void TCPServer::StartAccept()
{
	// this ensures that the TCPListener is never deleted during an active callback
	auto self(shared_from_this());
	auto callback = [self](std::error_code ec)
	{
		if (ec)
		{
			SIMPLE_LOG_BLOCK(self->logger, flags::INFO, ec.message().c_str());
			self->handler->OnShutdown(self);
		}
		else
		{
			const auto ID = self->session_id;
			++self->session_id;

			// method responsible for closing
			self->handler->AcceptConnection(ID, self->executor, std::move(self->socket));
			self->StartAccept();
		}
	};


	this->acceptor.async_accept(socket, this->executor->strand.wrap(callback));
}

}



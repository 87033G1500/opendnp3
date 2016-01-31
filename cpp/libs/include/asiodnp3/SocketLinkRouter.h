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

#ifndef ASIODNP3_SOCKETLINKROUTER_H
#define ASIODNP3_SOCKETLINKROUTER_H

#include <openpal/logging/LogRoot.h>

#include <opendnp3/link/LinkLayerParser.h>
#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/link/LinkChannelStatistics.h>

#include <asio.hpp>


namespace asiodnp3
{	
	class SocketLinkRouter final : public opendnp3::ILinkRouter, public std::enable_shared_from_this<SocketLinkRouter>
	{
	public:		

		static std::shared_ptr<SocketLinkRouter> Create(openpal::Logger logger, asio::ip::tcp::socket socket);

		virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session) override;		

	private:

		SocketLinkRouter(openpal::Logger logger, asio::ip::tcp::socket socket);

		openpal::Logger m_logger;		
		opendnp3::LinkChannelStatistics m_stats;
		opendnp3::LinkLayerParser m_parser;

		asio::ip::tcp::socket m_socket;
		asio::strand m_strand;
	};
}

#endif


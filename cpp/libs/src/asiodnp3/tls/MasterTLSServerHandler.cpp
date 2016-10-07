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

#include "asiodnp3/tls/MasterTLSServerHandler.h"

#include "asiodnp3/LinkSession.h"

#include "asiopal/tls/TLSStreamChannel.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

std::shared_ptr<MasterTLSServerHandler> MasterTLSServerHandler::Create(
    openpal::LogRoot root,
    std::shared_ptr<IListenCallbacks> callbacks,
    IResourceManager& manager)
{
	return std::make_shared<MasterTLSServerHandler>(std::move(root), callbacks, manager);
}

MasterTLSServerHandler::MasterTLSServerHandler(
    openpal::LogRoot root,
    std::shared_ptr<IListenCallbacks> callbacks,
    IResourceManager& manager) :
	root(std::move(root)),
	callbacks(callbacks),
	manager(manager)
{

}

bool MasterTLSServerHandler::AcceptConnection(uint64_t sessionid, const asio::ip::tcp::endpoint& remote)
{
	std::ostringstream oss;
	oss << remote;

	if (this->callbacks->AcceptConnection(sessionid, remote.address().to_string()))
	{
		FORMAT_LOG_BLOCK(root.logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());
		return true;
	}
	else
	{
		FORMAT_LOG_BLOCK(root.logger, flags::INFO, "Rejected connection from: %s", oss.str().c_str());
		return false;
	}
}

bool MasterTLSServerHandler::VerifyCallback(uint64_t sessionid, bool preverified, asio::ssl::verify_context& ctx)
{
	int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());

	if (!preverified)
	{
		FORMAT_LOG_BLOCK(this->root.logger, flags::WARN, "Error verifying certificate at depth: %d", depth);
		return preverified;
	}

	// lookup the subject name
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	char subjectName[512];
	X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 512);

	FORMAT_LOG_BLOCK(this->root.logger, flags::INFO, "Depth: %d - Verified certificate: %s", depth, subjectName);

	return this->callbacks->AcceptCertificate(
	           sessionid,
	           X509Info(
	               depth,
	               RSlice(cert->sha1_hash, SHA_DIGEST_LENGTH), // the thumbprint
	               std::string(subjectName)
	           )
	       );
}

void MasterTLSServerHandler::AcceptStream(uint64_t sessionid, const std::shared_ptr<StrandExecutor>& executor, std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream)
{
	LinkSession::Create(
	    root.Clone(SessionIdToString(sessionid).c_str()),
	    sessionid,
	    manager,
	    callbacks,
	    TLSStreamChannel::Create(executor->Fork(), stream)	// run the link session in a new strand
	);
}

std::string MasterTLSServerHandler::SessionIdToString(uint64_t sessionid)
{
	std::ostringstream oss;
	oss << "session-" << sessionid;
	return oss.str();
}

}


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
#include <asiopal/PhysicalLayerAsyncSerial.h>

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>

#include <asiopal/ASIOSerialHelpers.h>

using namespace asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncSerial::PhysicalLayerAsyncSerial(
        Logger aLogger,
		asio::io_service* apIOService,
        const SerialSettings& arSettings) :

	PhysicalLayerAsyncASIO(aLogger, apIOService),
	mSettings(arSettings),
	mPort(*apIOService)
{

}

/* Implement the actions */

void PhysicalLayerAsyncSerial::DoOpen()
{
	std::error_code ec;
	mPort.open(mSettings.mDevice, ec);	
	
	if (!ec) 
	{
		Configure(mSettings, mPort, ec);
		if(ec)
		{ 
			std::error_code ec2;
			mPort.close(ec2);
		}
	}

	//use post to simulate an async open operation
	executor.Post(std::bind(&PhysicalLayerAsyncSerial::OnOpenCallback, this, ec));
}

void PhysicalLayerAsyncSerial::DoClose()
{
	std::error_code ec;
	mPort.close(ec);
	if(ec) LOG_BLOCK(LogLevel::Warning, ec.message());
}

void PhysicalLayerAsyncSerial::DoOpenSuccess()
{
	LOG_BLOCK(LogLevel::Info, "Port successfully opened");
}

void PhysicalLayerAsyncSerial::DoAsyncRead(openpal::WriteBuffer& arBuffer)
{
	uint8_t* pBuff = arBuffer;
	mPort.async_read_some(buffer(arBuffer, arBuffer.Size()),
	                      strand.wrap([this, pBuff](const std::error_code& error, size_t numRead) {
							  this->OnReadCallback(error, pBuff, numRead);
						  }));	
}

void PhysicalLayerAsyncSerial::DoAsyncWrite(const ReadOnlyBuffer& arBuffer)
{
	async_write(mPort, buffer(arBuffer, arBuffer.Size()),
	            strand.wrap(
	                    std::bind(&PhysicalLayerAsyncSerial::OnWriteCallback,
	                              this,
	                              std::placeholders::_1,
								  arBuffer.Size())
	            ));
}

}




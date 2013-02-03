
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
#include "MockUpperLayer.h"

#include "BufferHelpers.h"

#include <opendnp3/ToHex.h>
#include <opendnp3/Logger.h>
#include <opendnp3/LoggableMacros.h>

#include <memory>

namespace opendnp3
{

MockUpperLayer::MockUpperLayer(Logger* apLogger) :
	Loggable(apLogger),
	IUpperLayer(apLogger)
{}

void MockUpperLayer::_OnReceive(const uint8_t* apData, size_t aLength)
{
	this->WriteToBuffer(apData, aLength);
	if(mOnReceiveHandler) mOnReceiveHandler(apData, aLength);
}

void MockUpperLayer::_OnSendSuccess()
{
	LOG_BLOCK(LEV_DEBUG, "OnSendSuccess");
	++mState.mSuccessCnt;
}

void MockUpperLayer::_OnSendFailure()
{
	LOG_BLOCK(LEV_DEBUG, "OnSendFailure");
	++mState.mFailureCnt;
}

void MockUpperLayer::_OnLowerLayerUp()
{
	LOG_BLOCK(LEV_DEBUG, "OnLowerLayerUp");
	++mState.mNumLayerUp;
}

void MockUpperLayer::_OnLowerLayerDown()
{
	LOG_BLOCK(LEV_DEBUG, "OnLowerLayerDown");
	++mState.mNumLayerDown;
}

void MockUpperLayer::SendDown(const uint8_t* apData, size_t aNumBytes)
{
	if(this->mpLowerLayer) mpLowerLayer->Send(apData, aNumBytes);
}

void MockUpperLayer::SendDown(const std::string& arHexData)
{
	HexSequence hs(arHexData);
	this->SendDown(hs.Buffer(), hs.Size());
}

}

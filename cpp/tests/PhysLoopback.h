
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
#ifndef __PHYS_LOOPBACK_H_
#define __PHYS_LOOPBACK_H_

#include <opendnp3/PhysicalLayerMonitor.h>
#include <opendnp3/CopyableBuffer.h>

namespace opendnp3
{

/**
*	Buffers and sends all bytes received to back on the same layer.
*/
class PhysLoopback : public PhysicalLayerMonitor
{
public:
	PhysLoopback(Logger*, IPhysicalLayerAsync*);

private:

	size_t mBytesRead;
	size_t mBytesWritten;

	CopyableBuffer mBuffer;

	void _OnReceive(const uint8_t*, size_t);
	void _OnSendSuccess(void);
	void _OnSendFailure(void);

	void OnPhysicalLayerOpenSuccessCallback(void);
	void OnPhysicalLayerOpenFailureCallback(void) {}
	void OnPhysicalLayerCloseCallback(void) {}

	void StartRead();
};

}

#endif

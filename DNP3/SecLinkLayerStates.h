
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
#ifndef __SEC_LINK_LAYER_STATES_H_
#define __SEC_LINK_LAYER_STATES_H_

#include <APL/Types.h>
#include <APL/Singleton.h>
#include <string>

#include "LinkLayer.h"

namespace apl
{
namespace dnp
{

class SecStateBase
{
public:

	/* Incoming messages to secondary station */

	void ResetLinkStates(LinkLayer*);
	void RequestLinkStatus(LinkLayer*);
	void UnconfirmedUserData(LinkLayer*, const uint8_t* apData, size_t aDataLength);

	virtual void TestLinkStatus(LinkLayer*, bool aFcb) = 0;
	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const uint8_t* apData, size_t aDataLength) = 0;


	//every concrete state implements this for logging purposes
	virtual std::string Name() const = 0;
};

////////////////////////////////////////////////////////
//	Class SLLS_UnReset
////////////////////////////////////////////////////////
class SLLS_NotReset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_NotReset);

	void TestLinkStatus(LinkLayer*, bool aFcb);
	void ConfirmedUserData(LinkLayer*, bool aFcb, const uint8_t* apData, size_t aDataLength);
};

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
class SLLS_Reset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_Reset);

	void TestLinkStatus(LinkLayer*, bool aFcb);
	void ConfirmedUserData(LinkLayer*, bool aFcb, const uint8_t* apData, size_t aDataLength);
};

}
} //end namepsace


#endif

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

#ifndef SECAUTH_IMASTERAPPLICATIONSA_H
#define SECAUTH_IMASTERAPPLICATIONSA_H

#include <opendnp3/master/IMasterApplication.h>

#include "secauth/UpdateKey.h"

#include <string>

namespace secauth
{

/**
	Extends the master application interface for vanilla DNP3 with additional methods required for secure authentication
*/
class IMasterApplicationSA : public opendnp3::IMasterApplication
{

public:

	/**
	* Called when the update key sequence completes. Update keys are automatically updated in memory, but
	* user code needs to persist these values to non-volatile memory.
	*/
	virtual void PersistNewUpdateKey(const std::string& username, opendnp3::User user, const UpdateKey& key) = 0;

};

}

#endif


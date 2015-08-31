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
#ifndef SECAUTH_MOCKMASTERAPPLICATIONSA_H
#define SECAUTH_MOCKMASTERAPPLICATIONSA_H

#include "secauth/master/IMasterApplicationSA.h"

#include <testlib/MockUTCTimeSource.h>

#include <vector>

namespace opendnp3
{

class MockMasterApplicationSA : public secauth::IMasterApplicationSA
{

public:

	virtual openpal::UTCTimestamp Now() override
	{
		return utc.Now();
	}

	virtual void OnTaskComplete(const TaskInfo& info) override
	{
		completions.push_back(info);
	}

	virtual void PersistNewUpdateKey(const std::string& username, opendnp3::User user, const secauth::UpdateKey& key) override
	{
		updateKeyCallbacks.push_back(username);
	}

	std::vector<TaskInfo> completions;

	std::vector<std::string> updateKeyCallbacks;

	testlib::MockUTCTimeSource utc;
};


}

#endif

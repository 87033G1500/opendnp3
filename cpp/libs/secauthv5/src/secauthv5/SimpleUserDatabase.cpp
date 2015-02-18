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

#include "SimpleUserDatabase.h"

using namespace openpal;

namespace secauthv5
{
		
bool SimpleUserDatabase::GetUpdateKey(const User& user, UpdateKeyType& type, openpal::ReadBufferView& key) const
{
	auto iter = this->keyMap.find(user.GetId());
	if (iter == keyMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.first;
		key = iter->second.second->ToReadOnly();
		return true;
	}
}

bool SimpleUserDatabase::GetUpdateKeyType(const User& user, UpdateKeyType& type) const
{
	auto iter = this->keyMap.find(user.GetId());
	if (iter == keyMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.first;		
		return true;
	}
}

void SimpleUserDatabase::ConfigureUser(const User& user, UpdateKeyType type, const openpal::ReadBufferView& key)
{
	keyMap[user.GetId()] = StoredUserData(type, std::make_unique<DynamicBuffer>(key));
}

}




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

#include "HMACMode.h"

using namespace opendnp3;

namespace secauthv5
{
	opendnp3::HMACType ToHMACType(HMACMode mode)
	{
		switch (mode)
		{
		case(HMACMode::SHA1_TRUNC_8) :
			return HMACType::HMAC_SHA1_TRUNC_8;
		case(HMACMode::SHA1_TRUNC_10) :
			return HMACType::HMAC_SHA1_TRUNC_10;
		case(HMACMode::SHA256_TRUNC_8) :
			return HMACType::HMAC_SHA256_TRUNC_8;
		default:
			return HMACType::HMAC_SHA256_TRUNC_16;
		}
	}	

	uint32_t GetTruncationSize(HMACMode mode)
	{
		switch (mode)
		{
			case(HMACMode::SHA1_TRUNC_8) :
				return 8;
			case(HMACMode::SHA1_TRUNC_10) :
				return 10;
			case(HMACMode::SHA256_TRUNC_8) :
				return 8;
			default:
				return 16;
		}
	}
}



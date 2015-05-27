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
#ifndef SECAUTH_HMACPROVIDER_H
#define SECAUTH_HMACPROVIDER_H

#include <cstdint>

#include "HMACMode.h"
#include "AuthConstants.h"

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/container/StaticBuffer.h>

namespace secauth
{
	class HMACProvider
	{
		public:
			HMACProvider(openpal::ICryptoProvider& provider, HMACMode mode);
			
			opendnp3::HMACType GetType() const;
			
			openpal::ReadBufferView Compute(const openpal::ReadBufferView& key, std::initializer_list<openpal::ReadBufferView> buffers);

			uint32_t OutputSize() const { return TRUNC_SIZE; }

		private:

			static openpal::IHMACAlgo& GetHMAC(openpal::ICryptoProvider& provider, HMACMode mode);

			HMACMode mode;
			openpal::IHMACAlgo* pHMAC;
			const uint32_t TRUNC_SIZE;
			openpal::StaticBuffer<AuthConstants::MAX_HMAC_OUTPUT_SIZE> buffer;
	};
}

#endif


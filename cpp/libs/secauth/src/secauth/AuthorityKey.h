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
#ifndef SECAUTH_AUTHORITYKEY_H
#define SECAUTH_AUTHORITYKEY_H

#include <openpal/crypto/SecureStaticBuffer.h>

#include <opendnp3/gen/UpdateKeyMode.h>

namespace secauth
{

/**
	Stores and provides read/write access to an update key
*/
class AuthorityKey
{
	public:

		enum class KeySize
		{
			S128,
			S256
		};

		static const uint8_t MIN_KEY_SIZE_128 = 16;
		static const uint8_t MAX_KEY_SIZE_256 = 32;
		
		/**
		* Construct an invalid default key
		*/
		AuthorityKey();

		/**
		* Test constructor that initializes a key with the same value for every byte
		*/
		AuthorityKey(uint8_t repeat, AuthorityKey::KeySize size);

		/**
		* Initialize the key base on a view
		*/
		AuthorityKey(const openpal::ReadBufferView& key);

		/**
		* Retrieve update key view
		*/
		openpal::ReadBufferView GetKeyView() const;					

		/**
		* returns true if the key is valid, false otherwise
		*/
		bool IsValid() const;

		/**
		* Only accepts 128 or 256 bit update keys
		*
		* returns true if the key was of valid size, false otherwise
		*/
		bool Initialize(const openpal::ReadBufferView& key);

	private:		

		static uint32_t NumBytes(KeySize size);

		static bool IsValidKeySize(uint32_t count);
	 		
		uint32_t m_size;
		openpal::SecureStaticBuffer<MAX_KEY_SIZE_256> m_buffer;
};

}

#endif


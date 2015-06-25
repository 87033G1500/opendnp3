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

#ifndef OSSLCRYPTO_AESKEYWRAP_H
#define OSSLCRYPTO_AESKEYWRAP_H

#include <openpal/logging/Logger.h>
#include <openpal/util/Uncopyable.h>
#include <openpal/crypto/IKeyWrapAlgo.h>

namespace osslcrypto
{

	enum class AESKeyLength : uint8_t
	{
		L128,
		L256
	};

	class AESKeyWrap : openpal::Uncopyable
	{
	public:

		static openpal::ReadBufferView WrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec);
		static openpal::ReadBufferView UnwrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec);
	};

	class AESKeyWrap128 : public openpal::IKeyWrapAlgo
	{
		virtual openpal::ReadBufferView WrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const override final
		{
			return AESKeyWrap::WrapKeyAES(AESKeyLength::L128, kek, input, output, ec);
		}

		virtual openpal::ReadBufferView UnwrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const override final
		{
			return AESKeyWrap::UnwrapKeyAES(AESKeyLength::L128, kek, input, output, ec);
		}
	};

	class AESKeyWrap256 : public openpal::IKeyWrapAlgo
	{
		virtual openpal::ReadBufferView WrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const override final
		{
			return AESKeyWrap::WrapKeyAES(AESKeyLength::L256, kek, input, output, ec);
		}

		virtual openpal::ReadBufferView UnwrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const override final
		{
			return AESKeyWrap::UnwrapKeyAES(AESKeyLength::L256, kek, input, output, ec);
		}
	};
}

#endif

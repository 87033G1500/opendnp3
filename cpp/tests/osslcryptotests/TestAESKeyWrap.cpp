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
#include <catch.hpp>

#include <osslcrypto/CryptoProvider.h>

#include <openpal/container/DynamicBuffer.h>

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

#include <thread>
#include <atomic>

#define SUITE(name) "AESKeyWrapTestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

/*
From rfc3394		- 128 bit kek w/ 128 bit data
KEK:				000102030405060708090A0B0C0D0E0F
Key Data :			00112233445566778899AABBCCDDEEFF
output Ciphertext:  1FA68B0A8112B447AEF34BD8FB5A7B82 9D3E862371D2CFE5
*/
TEST_CASE(SUITE("TestKeyWrap128KEK128Data"))
{	
	CryptoProvider provider;
	HexSequence kek("000102030405060708090A0B0C0D0E0F");
	HexSequence input("00112233445566778899AABBCCDDEEFF");		
	auto ciphertext = "1FA68B0A8112B447AEF34BD8FB5A7B829D3E862371D2CFE5";

	DynamicBuffer out(24);
	auto output = out.GetWriteBufferView();	
	REQUIRE(provider.WrapKeyAES128(kek, input, output));	
	REQUIRE(output.IsEmpty());	
	REQUIRE(ToHex(out.ToReadOnly().Take(24), false) == ciphertext);
	
}


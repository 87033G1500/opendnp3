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

#define SUITE(name) "SHATestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

auto inputString = "DEADBEEF";
auto digestSHA1 = "D78F8BB992A56A597F6C7A1FB918BB78271367EB";
auto digestSHA256 = "5F78C33274E43FA9DE5659265C1D917E25C03722DCB0B8D27DB8D5FEAA813953";

TEST_CASE(SUITE("SHA1"))
{			
	HexSequence input(inputString);
	DynamicBuffer output(20);

	CryptoProvider crypto;
	auto write = output.GetWriteBufferView();
	REQUIRE(crypto.CalcSHA1(input.ToReadOnly(), write));
	REQUIRE(write.IsEmpty());

	REQUIRE(ToHex(output.ToReadOnly(), false) == digestSHA1);
}

TEST_CASE(SUITE("IncrementalSHA1"))
{
	HexSequence input(inputString);
	DynamicBuffer output(20);

	CryptoProvider crypto;	
	
	auto provider = crypto.CreateSHA1Provider();
	
	for (int i = 0; i < 2; ++i)
	{
		auto write = output.GetWriteBufferView();
		REQUIRE(provider->Init());
		REQUIRE(provider->Add(input.ToReadOnly().Take(2)));
		REQUIRE(provider->Add(input.ToReadOnly().Skip(2)));
		REQUIRE(provider->Complete(write));		
		REQUIRE(write.IsEmpty());
		REQUIRE(ToHex(output.ToReadOnly(), false) == digestSHA1);
	}	
}

TEST_CASE(SUITE("SHA256"))
{
	HexSequence input(inputString);
	DynamicBuffer output(32);

	CryptoProvider crypto;
	auto write = output.GetWriteBufferView();
	REQUIRE(crypto.CalcSHA256(input.ToReadOnly(), write));
	REQUIRE(write.IsEmpty());

	REQUIRE(ToHex(output.ToReadOnly(), false) == digestSHA256);
}

TEST_CASE(SUITE("IncrementalSHA256"))
{
	HexSequence input(inputString);
	DynamicBuffer output(32);

	CryptoProvider crypto;

	auto provider = crypto.CreateSHA256Provider();

	for (int i = 0; i < 2; ++i)
	{
		auto write = output.GetWriteBufferView();
		REQUIRE(provider->Init());
		REQUIRE(provider->Add(input.ToReadOnly().Take(2)));
		REQUIRE(provider->Add(input.ToReadOnly().Skip(2)));
		REQUIRE(provider->Complete(write));
		REQUIRE(write.IsEmpty());
		REQUIRE(ToHex(output.ToReadOnly(), false) == digestSHA256);
	}
}
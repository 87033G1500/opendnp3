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

#include "BufferHelpers.h"
#include "HexConversions.h"

#include <opendnp3/objects/Group120Var5.h>

#include <openpal/util/ToHex.h>
#include <openpal/container/DynamicBuffer.h>

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "Group120Var5TestSuite - " name

TEST_CASE(SUITE("Parser rejects empty buffer"))
{
	HexSequence buffer("");

	Group120Var5 output;	
	REQUIRE(!Group120Var5::Read(buffer.ToReadOnly(), output));
}

TEST_CASE(SUITE("Parser accepts empty challenge data and hmac"))
{
	// SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 0
	HexSequence buffer("01 00 00 00 07 00 02 01 04 00 00");

	Group120Var5 output;
	REQUIRE(Group120Var5::Read(buffer.ToReadOnly(), output));
	REQUIRE(output.keyChangeSeqNum == 1);
	REQUIRE(output.userNum == 7);
	REQUIRE(output.keywrapAlgorithm == KeyWrapAlgorithm::AES_256);
	REQUIRE(output.keyStatus == KeyStatus::OK);
	REQUIRE(output.hmacType == HMACType::HMAC_SHA256_TRUNC_16);
	REQUIRE(output.challengeData.Size() == 0);
	REQUIRE(output.hmacValue.Size() == 0);
}

TEST_CASE(SUITE("Parser correctly interprets challenge data and hmac value"))
{
	// SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 3
	HexSequence buffer("01 00 00 00 07 00 02 01 04 03 00 DE AD BE EF");

	Group120Var5 output;
	REQUIRE(Group120Var5::Read(buffer.ToReadOnly(), output));	
	REQUIRE(toHex(output.challengeData) == "DE AD BE");
	REQUIRE(toHex(output.hmacValue) == "EF");
}

TEST_CASE(SUITE("Parser rejects one less than minimum required data"))
{
	// SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = ??? missing
	HexSequence buffer("01 00 00 00 07 00 02 01 04 00");

	Group120Var5 output;
	REQUIRE(!Group120Var5::Read(buffer.ToReadOnly(), output));	
}

TEST_CASE(SUITE("Parser rejects if specified challenge data is missing"))
{
	// SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 1, missing data
	HexSequence buffer("01 00 00 00 07 00 02 01 04 01 00");

	Group120Var5 output;
	REQUIRE(!Group120Var5::Read(buffer.ToReadOnly(), output));
}

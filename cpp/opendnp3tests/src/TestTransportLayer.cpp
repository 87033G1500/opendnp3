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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "TransportTestObject.h"
#include "Exception.h"

#include <opendnp3/transport/TransportConstants.h>

#include <opendnp3/Util.h>
#include <opendnp3/ProtocolUtil.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(TransportLayerTestSuite)

// make sure an invalid state exception gets thrown
// for every event other than LowerLayerUp() since
// the layer starts in the online state
BOOST_AUTO_TEST_CASE(StateOffline)
{
	TransportTestObject test;

	BOOST_REQUIRE_THROW(test.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendUp(""), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.ThisLayerDown(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(StateReady)
{
	TransportTestObject test(true); //makes an implicit call to 'test.lower.ThisLayerUp()'

	//check that that the transport layer is correctly forwarding up/down
	BOOST_REQUIRE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerDown();
	BOOST_REQUIRE_FALSE(test.upper.IsLowerLayerUp());
	test.lower.ThisLayerUp();
	BOOST_REQUIRE(test.upper.IsLowerLayerUp());

	//check that these actions all throw InvalidStateException
	BOOST_REQUIRE_THROW(test.lower.ThisLayerUp(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(ReceiveBadArguments)
{
	TransportTestObject test(true);	

	//check that the wrong aruments throw argument exceptions, and it's doesn't go to the sending state
	test.lower.SendUp("");
	BOOST_REQUIRE_EQUAL(TLERR_NO_PAYLOAD, test.log.NextErrorCode());
	test.lower.SendUp("FF");
	BOOST_REQUIRE_EQUAL(TLERR_NO_PAYLOAD, test.log.NextErrorCode());	

	test.lower.SendUp(test.GetData("C0", 0, 250)); // length 251

	BOOST_REQUIRE_EQUAL(TLERR_TOO_MUCH_DATA, test.log.NextErrorCode());
}

BOOST_AUTO_TEST_CASE(ReceiveNoPayload)
{
	TransportTestObject test(true);
	//try sending a FIR/FIN packet with no payload (1 byte)
	test.lower.SendUp("C0"); // FIR/FIN
	BOOST_REQUIRE_EQUAL(test.log.NextErrorCode(), TLERR_NO_PAYLOAD);
}

BOOST_AUTO_TEST_CASE(ReceiveNoFIR)
{
	TransportTestObject test(true);
	//try sending a non-FIR w/ no prior packet
	test.lower.SendUp("80 77"); // _/FIN
	BOOST_REQUIRE_EQUAL(test.log.NextErrorCode(), TLERR_MESSAGE_WITHOUT_FIR);
}

BOOST_AUTO_TEST_CASE(ReceiveWrongSequence)
{
	TransportTestObject test(true);
	//send a FIR, followed by a FIN w/ the wrong sequence
	test.lower.SendUp(test.GetData("40")); // FIR/_/0
	test.lower.SendUp(test.GetData("82")); // _/FIN/2
	BOOST_REQUIRE_EQUAL(test.log.NextErrorCode(), TLERR_BAD_SEQUENCE);
}

BOOST_AUTO_TEST_CASE(PacketsCanBeOfVaryingSize)
{
	TransportTestObject test(true);	
	test.lower.SendUp("40 0A 0B 0C"); // FIR/_/0
	BOOST_REQUIRE(test.log.IsLogErrorFree());
	test.lower.SendUp("81 0D 0E 0F"); // _/FIN/1
	BOOST_REQUIRE(test.log.IsLogErrorFree());
	BOOST_REQUIRE_EQUAL("0A 0B 0C 0D 0E 0F", test.upper.GetBufferAsHexString());
}

BOOST_AUTO_TEST_CASE(ReceiveSinglePacket)
{
	TransportTestObject test(true);
	//now try receiving 1 a single FIR/FIN with a magic value
	test.lower.SendUp("C0 77");
	BOOST_REQUIRE_EQUAL("77", test.upper.GetBufferAsHexString());	
}

BOOST_AUTO_TEST_CASE(ReceiveLargestPossibleAPDU)
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	for(string s: packets) {
		test.lower.SendUp(s);
	}

	BOOST_REQUIRE(test.log.IsLogErrorFree());
	BOOST_REQUIRE(test.upper.BufferEqualsHex(apdu)); //check that the correct data was written
}

BOOST_AUTO_TEST_CASE(ReceiveBufferOverflow)
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	//send 1 more packet than possible
	vector<string> packets;
	string apdu = test.GeneratePacketSequence(packets, num_packets + 1, last_packet_length);
for(string s: packets) {
		test.lower.SendUp(s);
	}

	BOOST_REQUIRE(test.upper.IsBufferEmpty());
	BOOST_REQUIRE_EQUAL(test.log.NextErrorCode(), TLERR_BUFFER_FULL);
}

BOOST_AUTO_TEST_CASE(ReceiveNewFir)
{
	TransportTestObject test(true);

	test.lower.SendUp(test.GetData("40"));	// FIR/_/0
	BOOST_REQUIRE(test.upper.IsBufferEmpty());

	test.lower.SendUp("C0 AB CD");	// FIR/FIN/0	
	BOOST_REQUIRE_EQUAL("AB CD", test.upper.GetBufferAsHexString());
	BOOST_REQUIRE_EQUAL(test.log.NextErrorCode(), TLERR_NEW_FIR); //make sure it logs the dropped frames
}

BOOST_AUTO_TEST_CASE(SendArguments)
{
	TransportTestObject test(true);
	BOOST_REQUIRE_THROW(test.upper.SendDown(""), ArgumentException); // 0 Length
	BOOST_REQUIRE_THROW(test.upper.SendDown(test.GetData("", 0, DEFAULT_FRAG_SIZE + 1)), ArgumentException); // Max Size + 1
}

BOOST_AUTO_TEST_CASE(StateSending)
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE_EQUAL("C0 11", test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0

	// Check that while we're sending, all other send requests are rejected
	BOOST_REQUIRE_THROW(test.upper.SendDown("00"), InvalidStateException);
	BOOST_REQUIRE_THROW(test.lower.ThisLayerUp(), InvalidStateException);

	//while we are sending, we should still be able to receive data as normal
	test.lower.SendUp("C0 77");
	test.upper.BufferEqualsHex("77");

	//this should put us back in the Ready state since it was a single tpdu send
	test.lower.SendSuccess();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 1);

	BOOST_REQUIRE_THROW(test.lower.SendSuccess(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(SendFailure)
{
	TransportTestObject test(true);

	test.lower.DisableAutoSendCallback();

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE_EQUAL("C0 11", test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0

	//this should put us back in the Ready state
	test.lower.SendFailure();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 0);
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mFailureCnt, 1);
	
	test.upper.SendDown("11");
	BOOST_REQUIRE_EQUAL("C0 11", test.lower.PopWriteAsHex()); // should resend with the same sequence number FIR/FIN SEQ=0
	test.lower.SendSuccess();
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 1);
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mFailureCnt, 1);
}

BOOST_AUTO_TEST_CASE(SendSuccess)
{
	TransportTestObject test(true);

	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE_EQUAL("C0 11", test.lower.PopWriteAsHex()); //FIR/FIN SEQ=0
	
	// this puts the layer into the Sending state
	test.upper.SendDown("11");
	BOOST_REQUIRE_EQUAL("C1 11", test.lower.PopWriteAsHex()); //FIR/FIN SEQ=1
	BOOST_REQUIRE_EQUAL(test.upper.GetState().mSuccessCnt, 2);
}

//if we're in the middle of a send and the layer goes down
BOOST_AUTO_TEST_CASE(ClosedWhileSending)
{
	TransportTestObject test(true);
	test.upper.SendDown("11"); //get the layer into the sending state

	test.lower.ThisLayerDown(); // go to the TS_ClosedAfterSend state
	BOOST_REQUIRE_FALSE(test.upper.IsLowerLayerUp());
}

BOOST_AUTO_TEST_CASE(SendFullAPDU)
{
	TransportTestObject test(true);

	uint32_t num_packets = CalcMaxPackets(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);
	uint32_t last_packet_length = CalcLastPacketSize(DEFAULT_FRAG_SIZE, TL_MAX_TPDU_PAYLOAD);

	vector<string> packets;
	std::string apdu = test.GeneratePacketSequence(packets, num_packets, last_packet_length);
	test.lower.DisableAutoSendCallback();
	test.upper.SendDown(apdu);

	//verify that each packet is received correctly
	for(string tpdu: packets) 
	{ 
		BOOST_REQUIRE_EQUAL(1, test.lower.NumWrites());
		BOOST_REQUIRE_EQUAL(tpdu, test.lower.PopWriteAsHex());
		BOOST_REQUIRE_EQUAL(0, test.lower.NumWrites());
		test.lower.SendSuccess();
	}
}


BOOST_AUTO_TEST_SUITE_END()


//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __TRANSPORT_SCALABILITY_TEST_OBJECT_H_
#define __TRANSPORT_SCALABILITY_TEST_OBJECT_H_

#include "TransportStackPair.h"
#include "AsyncTestObjectASIO.h"
#include "LogTester.h"

#include <asiopal/ASIOExecutor.h>

namespace opendnp3
{

class TransportScalabilityTestObject : public AsyncTestObjectASIO
{
public:
	TransportScalabilityTestObject(
	        LinkConfig aClientCfg,
	        LinkConfig aServerCfg,
	        boost::uint16_t aPortStart,
	        boost::uint16_t aNumPair,
	        openpal::FilterLevel aLevel = openpal::LEV_INFO,
	        bool aImmediate = false);

	~TransportScalabilityTestObject();

	void Start();


	// Test helpers
	bool AllLayersUp();
	bool AllLayerReceived(size_t aNumBytes);
	bool AllLayerEqual(const uint8_t*, size_t);

	void SendToAll(const uint8_t*, size_t);

	LogTester log;
	std::vector<TransportStackPair*> mPairs;
};

}

#endif

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

#include <opendnp3/OutstationEventBuffer.h>
#include <opendnp3/DynamicallyAllocatedEventBuffer.h>

#include "MeasurementComparisons.h"

#include "MockEventWriter.h"

using namespace std;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(OutstationBufferSuiteTestSuite)

BOOST_AUTO_TEST_CASE(InitialState)
{
	DynamicallyAllocatedEventBuffer underlying(3,3,3);
	OutstationEventBuffer buffer(underlying.GetFacade());

	MockEventWriter writer;

	SelectionCriteria criteria;	
	BOOST_REQUIRE_EQUAL(0, buffer.SelectEvents(criteria, &writer));
	BOOST_REQUIRE_EQUAL(0, buffer.SelectEvents(criteria, &writer));
	BOOST_REQUIRE_EQUAL(0, writer.TotalEvents());
}

BOOST_AUTO_TEST_CASE(SingleValueIsWrittenAndCleared)
{
	DynamicallyAllocatedEventBuffer underlying(3,3,3);
	OutstationEventBuffer buffer(underlying.GetFacade());

	buffer.Update(Event<Binary>(Binary(true), 3, PC_CLASS_1));

	MockEventWriter writer;

	SelectionCriteria criteria;
	criteria.AllEvents();
	
	BOOST_REQUIRE_EQUAL(1, buffer.SelectEvents(criteria, &writer));
	BOOST_REQUIRE_EQUAL(1, writer.TotalEvents());
	BOOST_REQUIRE_EQUAL(0, buffer.SelectEvents(criteria, &writer)); //second select does nothing
	BOOST_REQUIRE_EQUAL(1, writer.TotalEvents());

	BOOST_REQUIRE_EQUAL(1, buffer.SelectedEvents().class1.numBinary);
	BOOST_REQUIRE_EQUAL(0, buffer.SelectedEvents().class2.numBinary);
	BOOST_REQUIRE_EQUAL(0, buffer.SelectedEvents().class3.numBinary);
	BOOST_REQUIRE_EQUAL(0, buffer.SelectedEvents().class1.numAnalog);

	BOOST_REQUIRE_EQUAL(0, buffer.UnselectedEvents().class1.numBinary);

	buffer.Reset();

	BOOST_REQUIRE_EQUAL(1, buffer.SelectEvents(criteria, &writer)); //event goes right back into buffer
	BOOST_REQUIRE_EQUAL(2, writer.TotalEvents());
	BOOST_REQUIRE(writer.binaries[0] == writer.binaries[1]); // same event

	buffer.Clear();

	BOOST_REQUIRE(buffer.TotalEvents().IsEmpty());
	BOOST_REQUIRE(buffer.SelectedEvents().IsEmpty());
	BOOST_REQUIRE(buffer.UnselectedEvents().IsEmpty());
}

BOOST_AUTO_TEST_CASE(MixedTypesAndClassesOfEvents)
{
	DynamicallyAllocatedEventBuffer underlying(2,2,2);
	OutstationEventBuffer buffer(underlying.GetFacade());

	// reasonable mix of events that fills up the buffer
	buffer.Update(Event<Binary>(Binary(true), 3, PC_CLASS_1));
	buffer.Update(Event<Binary>(Binary(true), 5, PC_CLASS_3));
	buffer.Update(Event<Analog>(Analog(16), 1, PC_CLASS_2));
	buffer.Update(Event<Analog>(Analog(71), 7, PC_CLASS_3));
	buffer.Update(Event<Counter>(Counter(23), 3, PC_CLASS_2));
	buffer.Update(Event<Counter>(Counter(42), 4, PC_CLASS_2));
		
	MockEventWriter writer;
	SelectionCriteria criteria;
}

BOOST_AUTO_TEST_SUITE_END()
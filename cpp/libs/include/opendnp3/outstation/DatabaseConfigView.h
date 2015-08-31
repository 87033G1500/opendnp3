/*
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
#ifndef OPENDNP3_DATABASECONFIGVIEW_H
#define OPENDNP3_DATABASECONFIGVIEW_H

#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/outstation/Cell.h"

#include <openpal/container/ArrayView.h>

namespace opendnp3
{

/**
* DatabaseConfigView provides abstracted access to the raw buffers in outstation database.
*
* Use this object to congfigure:
*
*  1) Inital values if you want something besides false/zero with 0x02 restart quality
*  2) Default static/event reporting variations for each point
*  3) Class assignments (0,1,2,3) for each point
*  4) deadbands for analogs / counters / etc
*
*/
class DatabaseConfigView
{

public:

	DatabaseConfigView(
	    openpal::ArrayView<Cell<Binary>, uint16_t> binaries_,
	    openpal::ArrayView<Cell<DoubleBitBinary>, uint16_t> doubleBinaries_,
	    openpal::ArrayView<Cell<Analog>, uint16_t> analogs_,
	    openpal::ArrayView<Cell<Counter>, uint16_t> counters_,
	    openpal::ArrayView<Cell<FrozenCounter>, uint16_t> frozenCounters_,
	    openpal::ArrayView<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii_,
	    openpal::ArrayView<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii_,
	    openpal::ArrayView<Cell<TimeAndInterval>, uint16_t> timeAndIntervals_
	);

	// ------------ Helper functions for setting initial value ------

	void SetInitialValue(const Binary& meas, uint16_t index);
	void SetInitialValue(const DoubleBitBinary& meas, uint16_t index);
	void SetInitialValue(const Analog& meas, uint16_t index);
	void SetInitialValue(const Counter& meas, uint16_t index);
	void SetInitialValue(const FrozenCounter& meas, uint16_t index);
	void SetInitialValue(const BinaryOutputStatus& meas, uint16_t index);
	void SetInitialValue(const AnalogOutputStatus& meas, uint16_t index);
	void SetInitialValue(const TimeAndInterval& meas, uint16_t index);

	//  ----------- Views of the underlying storage ---------

	openpal::ArrayView<Cell<Binary>, uint16_t> binaries;
	openpal::ArrayView<Cell<DoubleBitBinary>, uint16_t> doubleBinaries;
	openpal::ArrayView<Cell<Analog>, uint16_t> analogs;
	openpal::ArrayView<Cell<Counter>, uint16_t> counters;
	openpal::ArrayView<Cell<FrozenCounter>, uint16_t> frozenCounters;
	openpal::ArrayView<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii;
	openpal::ArrayView<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii;
	openpal::ArrayView<Cell<TimeAndInterval>, uint16_t> timeAndIntervals;
};

}

#endif

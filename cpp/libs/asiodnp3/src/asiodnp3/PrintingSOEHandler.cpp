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

#include "PrintingSOEHandler.h"

using namespace std;

namespace opendnp3
{

PrintingSOEHandler PrintingSOEHandler::instance;

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values)
{
	auto print = [](const Indexed<OctetString>& pair) {
		std::cout << "OctetString " << " [" << pair.index << "] : Size : " << pair.value.ToReadOnly().Size() << std::endl;
	};

	values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values)
{
	auto print = [](const Indexed<TimeAndInterval>& pair) {
		std::cout << "TimeAndInterval: " <<
			"[" << pair.index << "] : " <<
			pair.value.time << " : " <<
			pair.value.interval << " : " <<
			IntervalUnitsToString(pair.value.GetUnitsEnum()) << std::endl;
	};

	values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	auto print = [](const Indexed<BinaryCommandEvent>& pair) {
		std::cout << "BinaryCommandEvent: " <<
			"[" << pair.index << "] : " <<
			pair.value.time << " : " <<
			pair.value.value << " : " <<
			CommandStatusToString(pair.value.status) << std::endl;
	};

	values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	auto print = [](const Indexed<AnalogCommandEvent>& pair) {
		std::cout << "AnalogCommandEvent: " <<
			"[" << pair.index << "] : " <<
			pair.value.time << " : " <<
			pair.value.value << " : " <<
			CommandStatusToString(pair.value.status) << std::endl;
	};

	values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values)
{
	auto print = [](const Indexed<SecurityStat>& pair) {
		std::cout << "SecurityStat: " <<
			"[" << pair.index << "] : " <<
			pair.value.time << " : " <<
			pair.value.count << " : " <<
			static_cast<int>(pair.value.quality) << " : " <<
			pair.value.assocId << std::endl;
	};

	values.ForeachItem(print);
}


/*

void PrintingSOEHandler::OnValue(const HeaderInfo& info, const BinaryCommandEvent& meas, uint16_t index)
{
	std::cout << "BinaryCommandEvent: " <<
		"[" << index << "] : " <<
		meas.time << " : " <<
		meas.value << " : " <<
		CommandStatusToString(meas.status) << std::endl;
}

void PrintingSOEHandler::OnValue(const HeaderInfo& info, const AnalogCommandEvent& meas, uint16_t index)
{
	std::cout << "AnalogCommandEvent: " <<
		"[" << index << "] : " <<
		meas.time << " : " <<
		meas.value << " : " <<
		CommandStatusToString(meas.status) << std::endl;
}

void PrintingSOEHandler::OnValue(const HeaderInfo& info, const SecurityStat& meas, uint16_t index)
{
	std::cout << "SecurityStat: " <<
		"[" << index << "] : " <<
		meas.time << " : " <<
		meas.count << " : " <<
		static_cast<int>(meas.quality) << " : " <<
		meas.assocId << std::endl;
}
*/

}

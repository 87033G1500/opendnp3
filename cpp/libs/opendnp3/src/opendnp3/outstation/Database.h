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
#ifndef OPENDNP3_DATABASE_H
#define OPENDNP3_DATABASE_H

#include <openpal/executor/IMutex.h>

#include "opendnp3/app/Range.h"
#include "opendnp3/gen/AssignClassType.h"

#include "opendnp3/outstation/DatabaseTemplate.h"
#include "opendnp3/outstation/StaticBuffers.h"
#include "opendnp3/outstation/IDatabase.h"
#include "opendnp3/outstation/IEventReceiver.h"


namespace opendnp3
{

class INewEventDataHandler
{
public:
	// called when a transaction produces new event data
	virtual void OnNewEventData() = 0;
};

/**
The database coordinates all updates of measurement data
*/
class Database : public IDatabase
{
public:

	Database(const DatabaseTemplate&, IEventReceiver& eventReceiver, INewEventDataHandler& handler, openpal::IMutex* pMutex);

	// IMeasurementUpdater functions
	bool Update(const Binary& value, uint16_t) override final;
	bool Update(const DoubleBitBinary& value, uint16_t) override final;
	bool Update(const Analog& value, uint16_t) override final;
	bool Update(const Counter& value, uint16_t) override final;
	bool Update(const FrozenCounter& value, uint16_t) override final;
	bool Update(const BinaryOutputStatus& value, uint16_t) override final;
	bool Update(const AnalogOutputStatus& value, uint16_t) override final;
	bool Update(const TimeAndInterval& value, uint16_t) override final;	

	bool AssignClass(AssignClassType type, PointClass clazz, const Range& range);	
	

	StaticBufferView GetStaticView() { return staticBuffers.GetView(); }

private:

	// stores the most revent values and event information
	StaticBuffers staticBuffers;


	IEventReceiver* pEventReceiver;
	openpal::IMutex* pMutex;
		
	INewEventDataHandler* pEventHandler;
	bool transactionHasEvents;

	Database() = delete;
	Database(const Database&) = delete;

	static bool ConvertToEventClass(PointClass pc, EventClass& ec);	

	template <class T>
	bool UpdateEvent(const T& value, uint16_t index, openpal::ArrayView<MeasurementCell<T>, uint16_t>& values);

	// ITransactable  functions, proxies to the given transactable
	virtual void Start() override final;
	virtual void End() override final;
};

template <class T>
bool Database::UpdateEvent(const T& value, uint16_t index, openpal::ArrayView<MeasurementCell<T>, uint16_t>& values)
{	
	if (values.Contains(index))
	{
		auto& metadata = values[index].metadata;

		EventClass ec;
		if (ConvertToEventClass(metadata.clazz, ec) && metadata.IsEvent(value))
		{
			metadata.lastEvent = value;

			if (pEventReceiver)
			{
				pEventReceiver->Update(Event<T>(value, index, ec));
				transactionHasEvents = true;
			}
		}

		values[index].currentValue = value;
		return true;
	}
	else
	{
		return false;
	}
}

}

#endif


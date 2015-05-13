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
#ifndef OPENDNP3_APDUHANDLERBASE_H
#define OPENDNP3_APDUHANDLERBASE_H

#include "opendnp3/app/IINField.h"

#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>



namespace opendnp3
{

/**
 * Base class used to handle APDU object headers
 */
class APDUHandlerBase : public IAPDUHandler
{
public:

	/**
	 * @param logger the Logger that the loader should use for message reporting
	 */
	APDUHandlerBase(openpal::Logger logger);

	uint32_t NumIgnoredHeaders() const
	{
		return numIgnoredHeaders;
	}

	IINField Errors() const;

	virtual void AllObjects(const HeaderRecord& record) override final;

	virtual void OnRangeRequest(const HeaderRecord& record, const Range& range) override final;

	virtual void OnCountRequest(const HeaderRecord& record, uint16_t count) override final;

	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var5& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final;

	virtual void OnCount(const HeaderRecord& record, const ICollection<Group50Var1>& values) override final;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group51Var1>& values) override final;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group51Var2>& values) override final;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group52Var2>& values) override final;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group120Var4>& values) override final;

	virtual void OnRange(const HeaderRecord& record, uint32_t count, const IINValue& value, uint16_t index) override final;

	virtual void OnRange(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const TimeAndInterval& meas, uint16_t index) override final;
	virtual void OnRange(const HeaderRecord& record, uint32_t count, const Group121Var1& meas, uint16_t index) override final;

	// events

	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const TimeAndInterval& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& meas, uint16_t index) override final;	
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var1& meas, uint16_t index) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var2& meas, uint16_t index) override final;

	// commands

	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& meas) override final;

protected:

	void Reset();	

	inline uint32_t GetCurrentHeader()
	{
		return numTotalHeaders;
	}

	inline bool IsFirstHeader()
	{
		return numTotalHeaders == 0;
	}


	virtual IINField ProcessAllObjects(const HeaderRecord& record);
	virtual IINField ProcessRangeRequest(const HeaderRecord& record, const Range& range);
	virtual IINField ProcessCountRequest(const HeaderRecord& record, uint16_t count);

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value);
	
	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group50Var1>& values);
	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group51Var1>& values);
	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group51Var2>& values);
	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group52Var2>& values);
	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group120Var4>& values);
	
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const IINValue& value, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const TimeAndInterval& meas, uint16_t index);
	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Group121Var1& meas, uint16_t index);

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const TimeAndInterval& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& meas, uint16_t index);	
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var1& meas, uint16_t index);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var2& meas, uint16_t index);

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>> &meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& meas);

protected:

	openpal::Logger logger;	

	// overridable to receive events for every header
	virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) {}
	
private:

	inline void Record(const HeaderRecord& record, const IINField& result)
	{
		errors |= result;
		++numTotalHeaders;
		this->OnHeaderResult(record, result);
	}

	inline IINField ProcessUnsupportedHeader()
	{
		++numIgnoredHeaders;
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}

	IINField errors;
	uint32_t numTotalHeaders;
	uint32_t numIgnoredHeaders;			
};

}



#endif


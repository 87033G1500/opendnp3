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
#ifndef __MOCK_APDU_HEADER_HANDLER_H_
#define __MOCK_APDU_HEADER_HANDLER_H_

#include <opendnp3/app/parsing/APDUHandlerBase.h>

#include <testlib/MockLogHandler.h>

#include <vector>

namespace opendnp3
{

class MockApduHeaderHandler : public testlib::MockLogHandler, public APDUHandlerBase
{
public:

	MockApduHeaderHandler() : MockLogHandler(), APDUHandlerBase(this->GetLogger())
	{}

	virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) override final
	{
		records.push_back(record);
	}	

	virtual IINField ProcessCount(const HeaderRecord& record, const ICollection<Group120Var4>& values) override final
	{	
		return ProcessAny(record, values, authStatusRequests);
		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<IINValue>>& values) override final
	{
		return ProcessAny(record, values, iinBits);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values) override final
	{	
		return ProcessAny(record, values, staticBinaries);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values) override final
	{				
		return ProcessAny(record, values, staticDoubleBinaries);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values) override final
	{		
		return ProcessAny(record, values, staticControlStatii);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values) override final
	{				
		return ProcessAny(record, values, staticCounters);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values) override final
	{				
		return ProcessAny(record, values, staticFrozenCounters);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values) override final
	{		
		return ProcessAny(record, values, eventAnalogs);		
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values) override final
	{		
		return ProcessAny(record, values, staticSetpointStatii);
	}

	virtual IINField ProcessRange(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values) override final
	{				
		return ProcessAny(record, values, rangedOctets);		
	}
	
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values) override final
	{			
		return this->ProcessAny(record, values, eventBinaries);		
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values) override final
	{				
		return this->ProcessAny(record, values, eventDoubleBinaries);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values) override final
	{		
		return this->ProcessAny(record, values, eventCounters);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values) override final
	{		
		return this->ProcessAny(record, values, eventFrozenCounters);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values) override final
	{		
		return this->ProcessAny(record, values, eventAnalogs);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values) override final
	{				
		return this->ProcessAny(record, values, indexPrefixedOctets);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryCommandEvent>>& values) override final
	{				
		return this->ProcessAny(record, values, binaryCommandEvents);		
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogCommandEvent>>& values) override final
	{				
		return this->ProcessAny(record, values, analogCommandEvents);
	}

	/// --- controls ----

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& meas) override final
	{			
		return this->ProcessAny(record, meas, crobRequests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoInt16Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoInt32Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoFloat32Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoDouble64Requests);
	}	

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final
	{	
		authChallenges.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final
	{	
		authReplys.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value) override final
	{	
		authKeyStatusResponses.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final
	{
		authChanges.push_back(value);
		return IINField::Empty();
	}

	std::vector<HeaderRecord> records;

	std::vector<Group120Var1> authChallenges;
	std::vector<Group120Var2> authReplys;
	std::vector<Group120Var4> authStatusRequests;
	std::vector<Group120Var5> authKeyStatusResponses;
	std::vector<Group120Var6> authChanges;

	std::vector<Indexed<IINValue>> iinBits;

	std::vector<Indexed<Binary>> eventBinaries;
	std::vector<Indexed<Binary>> staticBinaries;

	std::vector<Indexed<DoubleBitBinary>> eventDoubleBinaries;
	std::vector<Indexed<DoubleBitBinary>> staticDoubleBinaries;

	std::vector<Indexed<BinaryOutputStatus>> staticControlStatii;

	std::vector<Indexed<Counter>> eventCounters;
	std::vector<Indexed<Counter>> staticCounters;

	std::vector<Indexed<FrozenCounter>> eventFrozenCounters;
	std::vector<Indexed<FrozenCounter>> staticFrozenCounters;

	std::vector<Indexed<Analog>> eventAnalogs;
	std::vector<Indexed<Analog>> staticAnalogs;

	std::vector<Indexed<AnalogOutputStatus>> staticSetpointStatii;

	std::vector<Indexed<ControlRelayOutputBlock>> crobRequests;

	std::vector<Indexed<AnalogOutputInt16>> aoInt16Requests;
	std::vector<Indexed<AnalogOutputInt32>> aoInt32Requests;
	std::vector<Indexed<AnalogOutputFloat32>> aoFloat32Requests;
	std::vector<Indexed<AnalogOutputDouble64>> aoDouble64Requests;

	std::vector<Indexed<OctetString>> indexPrefixedOctets;
	std::vector<Indexed<OctetString>> rangedOctets;

	std::vector<Indexed<BinaryCommandEvent>> binaryCommandEvents;

	std::vector<Indexed<AnalogCommandEvent>> analogCommandEvents;

	private:

	template <class T>
	IINField ProcessAny(const HeaderRecord& record, const ICollection<T>& meas, std::vector<T>& items)
	{
		auto add = [&items](const T& v) { items.push_back(v); };
		meas.ForeachItem(add);
		return IINField::Empty();
	}
};

}

#endif

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
#ifndef __COMMAND_SEQUENCE_H_
#define __COMMAND_SEQUENCE_H_

#include "APDUHandlerBase.h"

namespace opendnp3
{

/**
*  Abstracts information for 
*/
class ICommandSequence : public APDUHandlerBase, private openpal::Loggable
{
	public:

	ICommandSequence(openpal::Logger logger): Loggable(logger) {}

	// Given an APDU and function code, configure the request
	// virtual void FormatAPDU(APDU& apdu, FunctionCode aCode) = 0;  TODO

	// Given the response, what's the result of the command?
	virtual CommandResponse Validate() = 0;
};

template <class CommandType, class GroupVariation>
class CommandSequence : public ICommandSequence
{
	public:
	CommandSequence(openpal::Logger logger) : ICommandSequence(logger) {}	

	void Configure(const CommandType& value, uint32_t index) 
	{ 
		this->Reset(); // resets all state inside the base class
		response = CommandResponse(CommandResult::TIMEOUT); // todo change this to some other result like "malformed"
		command = IndexedValue<CommandType>(value, index); 
	}

	virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<CommandType>>& meas)
	{		
		if(this->IsFirstHeader())
		{						
			IndexedValue<CommandType> received;
			if(meas.ReadOnlyValue(received))
			{
				if(received.index == command.index && received.value.ValuesEqual(command.value))
				{
					response = CommandResponse(CommandResult::RESPONSE_OK, received.value.status);				
				}
			}									
		}				
	}

	/* TODO
	virtual void FormatAPDU(APDU& apdu, FunctionCode aCode) final 
	{		
		apdu.Set(aCode, true, true, false, false);
		IndexedWriteIterator i = apdu.WriteIndexed(GroupVariation::Inst(), 1, command.index);
		i.SetIndex(command.index);
		GroupVariation::Inst()->Write(*i, command.value);
	}
	*/

	virtual CommandResponse Validate() final
	{
		auto numHeaders = this->GetCurrentHeader();
		auto singleHeader = (numHeaders == 1);
		this->Reset();
		if(singleHeader) return response;
		else return CommandResponse(CommandResult::TIMEOUT); // TODO - better error code
	}

	private:
	CommandResponse response;
	IndexedValue<CommandType> command;
};


} //ens ns

#endif

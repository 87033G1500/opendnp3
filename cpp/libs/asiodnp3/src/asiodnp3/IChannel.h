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
#ifndef __I_CHANNEL_H_
#define __I_CHANNEL_H_

#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/link/LinkChannelStatistics.h>

#include <openpal/executor/IUTCTimeSource.h>
#include <openpal/logging/LogFilters.h>
#include <openpal/executor/IExecutor.h>

#include "DestructorHook.h"

namespace opendnp3
{
	class ISOEHandler;
	class ICommandHandler;
	class ITimeWriteHandler;
}

namespace asiodnp3
{

class IMaster;
class IOutstation;


/**
* Represents a communication channel upon which masters and outstations can be bound.
* To add a master to a communication channel, where pClient is a channel pointer IChannel*\n
* Returns a pointer to the running master
\code
	IMaster* pMaster = pClient->AddMaster(
		"master",						// stack name
		LOG_LEVEL,						// log filter level
		PrintingDataObserver::Inst(),	// callback for data processing
		stackConfig						// stack configuration
	);
\endcode
* To add an outstation to a communication channel, where pServer is a channel pointer IChannel*\n
* Returns a pointer to the running outstation
\code
	IOutstation* pOutstation = pServer->AddOutstation(
		"outstation",								// stack name
		LOG_LEVEL,									// log filter level
		SuccessCommandHandler::Inst(),				// callback for command requests
		stackConfig									// stack configuration
	);
\endcode
*/
class IChannel : public DestructorHook
{
public:	
	
	virtual ~IChannel() {}

	/*
	* Receive callbacks for state transitions on the channels executor
	* You'll receive one callback immediately with the current state
	*/
	virtual void AddStateListener(const std::function<void(opendnp3::ChannelState)>& listener) = 0;
	
	/**
	* Synchronously read the channel statistics
	*/
	virtual opendnp3::LinkChannelStatistics GetChannelStatistics() = 0;
	
	/**
	* sychronously shutdown the channel
	*/
	virtual void Shutdown() = 0;	

	/**
	*  @return The current logger settings for this channel
	*/
	virtual openpal::LogFilters GetLogFilters() const = 0;	

	/**
	*  @param filters Adjust the filters to this value
	*/
	virtual void SetLogFilters(const openpal::LogFilters& filters) = 0;

	/**
	* Add a master to the channel
	*
	* @param id An ID that gets used for logging
	* @param pPublisher Callback object for all received measurements
	* @param pTimeSource The interface that is used for outstation time synchronization
	* @param config Configuration object that controls how the master behaves
	* @return interface representing the running master
	*/
	virtual IMaster* AddMaster(char const* id,
	                           opendnp3::ISOEHandler* pPublisher,
	                           openpal::IUTCTimeSource* pTimeSource,
							   const opendnp3::MasterStackConfig& config) = 0;

	/**
	* Add an outstation to the channel
	*
	* @param id An ID that gets used for logging
	* @param pCmdHandler Callback object for handling command requests
	* @param pTimeWriteHandler Interface used to receive time synchronization requests
	* @param config Configuration object that controls how the outstation behaves
	* @return interface representing the running outstations
	*/
	virtual IOutstation* AddOutstation( char const* id,
										opendnp3::ICommandHandler* pCmdHandler,
										opendnp3::ITimeWriteHandler* pTimeWriteHandler,
										const opendnp3::OutstationStackConfig& config) = 0;
};

}

#endif

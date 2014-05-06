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
#ifndef __I_MASTER_H_
#define __I_MASTER_H_

#include "DNP3Stack.h"
#include "StackActionHandler.h"

#include <opendnp3/master/MasterScan.h>
#include <openpal/TimeDuration.h>

namespace opendnp3
{
	class ICommandProcessor;
}

namespace asiodnp3
{

/** Interface that represents a running master.
* To get a command processor interface to execute controls on the master:-
\code
	ICommandProcessor* pCmdProcessor = pMaster->GetCommandProcessor();
\endcode
*/
class IMaster : public DNP3Stack
{
public:
	IMaster(openpal::LogRoot& root, openpal::IExecutor* pExecutor, opendnp3::LinkConfig linkConfig, const StackActionHandler& handler) :
		DNP3Stack(root, pExecutor, linkConfig, handler)
	{}

	virtual ~IMaster() {}

	/**
	*  Returns a master scan object representing the configured integrity scan
	*/
	virtual opendnp3::MasterScan GetIntegrityScan() = 0;

	/**
	* Add a class-based scan to the master
	* @return A proxy class used to manipulate the scan
	*/
	virtual opendnp3::MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate) = 0;

	/**
	* Get a command processor interface to execute controls on the master
	* @return Interface used to invoke commands
	*/
	virtual opendnp3::ICommandProcessor* GetCommandProcessor() = 0;
};

}

#endif


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
#ifndef __MASTER_STACK_IMPL_H_
#define __MASTER_STACK_IMPL_H_

#include "IMaster.h"

#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/Master.h>
#include <opendnp3/transport/TransportStack.h>

#include <openpal/IUTCTimeSource.h>

namespace asiodnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class MasterStackImpl : public IMaster
{
public:

	MasterStackImpl(
		openpal::LogRoot& root,
	    openpal::IExecutor& executor,
		opendnp3::ISOEHandler* pSOEHandler,
	    openpal::IUTCTimeSource* pTimeSource,		
		const opendnp3::MasterStackConfig& config,
		const StackActionHandler& handler);

	virtual void Enable() override final;

	virtual void Disable() override final;

	virtual void BeginShutdown() override final;

	virtual opendnp3::StackStatistics GetStackStatistics() override final;	

	virtual openpal::IExecutor* GetExecutor() override final;

	virtual opendnp3::ICommandProcessor* GetCommandProcessor()  override final;

	// ------- Scan handlers ---------

	virtual opendnp3::MasterScan AddClassScan(uint8_t classMask, openpal::TimeDuration period) override final;

	virtual opendnp3::MasterScan  AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period) override final;

	// ------- Non-interface public members ---------

	void SetLinkRouter(opendnp3::ILinkRouter* pRouter);

	opendnp3::ILinkContext* GetLinkContext();


private:
	opendnp3::StackStatistics statistics;
	StackActionHandler handler;
	opendnp3::TransportStack stack;
	opendnp3::Master master;
};

}

#endif


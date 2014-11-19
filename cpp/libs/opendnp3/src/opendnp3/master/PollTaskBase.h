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
#ifndef OPENDNP3_POLLTASKBASE_H
#define OPENDNP3_POLLTASKBASE_H

#include "opendnp3/master/IMasterTask.h"

#include <string>

namespace opendnp3
{

class ISOEHandler;

/**
 * Base class for measurement polls
 */
class PollTaskBase : public IMasterTask, openpal::Uncopyable
{	

public:		

	PollTaskBase(IMasterApplication& application, ISOEHandler& soeHandler, openpal::MonotonicTimestamp expiration, openpal::Logger logger);

	virtual const char* Name() const override final { return "Application Poll"; };
	
protected:	

	virtual ResponseResult _OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects) override final;

	ResponseResult ProcessMeasurements(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects);

	virtual void Initialize() override final;		
		
	ISOEHandler* pSOEHandler;
	uint16_t rxCount;
};

} //end ns


#endif

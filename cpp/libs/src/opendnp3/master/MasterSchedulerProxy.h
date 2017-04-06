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

#ifndef OPENDNP3_MASTERSCHEDULERPROXY_H
#define OPENDNP3_MASTERSCHEDULERPROXY_H

#include "opendnp3/master/IMasterScheduler.h"
#include "opendnp3/master/IMasterTaskRunner.h"

namespace opendnp3
{

// forward declare the backend so we can erase its type
class MasterSchedulerBackend;

/**
* Acts as a proxy to the backend scheduler so that session don't have to be aware there are other sessions
*/
class MasterSchedulerProxy : public IMasterScheduler
{

public:

	MasterSchedulerProxy(
	    const std::shared_ptr<IMasterTaskRunner>& runner,
	    const std::shared_ptr<MasterSchedulerBackend>& backend
	) :
		runner(runner),
		backend(backend)
	{}

	~MasterSchedulerProxy();


	virtual void Add(std::initializer_list<std::shared_ptr<IMasterTask>> tasks) override;

	virtual void Add(const std::shared_ptr<IMasterTask>& tasks) override;

	virtual void Clear() override;

private:

	const std::shared_ptr<IMasterTaskRunner> runner;
	const std::shared_ptr<MasterSchedulerBackend> backend;
};

}

#endif

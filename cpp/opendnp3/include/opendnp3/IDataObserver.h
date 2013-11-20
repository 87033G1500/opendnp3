
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __I_DATA_OBSERVER_H_
#define	__I_DATA_OBSERVER_H_

#include "DataTypes.h"
#include "ITransactable.h"


namespace opendnp3
{

/**

IDataObserver is the key interface between master/outstation and
the "application" code. The outstation application is responsible for measuring or
calculating data and then pushing it into this interface when it is ready
to publish it. The master application is responsible for handling the data
as it is received from an outstation. That data needs to be strongly typed
and passed by index. As with all ITransactables it should be used with the
exception safe Transaction object.

\code
{
	Transaction t(pObserver);
	pObserver->Update(Analog(12), 3);
	pObserver->Update(Binary(true), 1);
}
\endcode

*/
class IDataObserver : public ITransactable
{
public:

	virtual ~IDataObserver() {}

	// NVII enforces a policy of using these functions only after
	// a transaction has been initiated

	/**
	* Update or receive a Binary measurement, must have transaction started
	* @param arMeas measurement to be processed
	* @param aIndex index of the measurement
	*/
	void Update(const Binary& arMeas, size_t aIndex);

	/**
	* Update or receive an Analog measurement, must have transaction started
	* @param arMeas measurement to be processed
	* @param aIndex index of the measurement
	*/
	void Update(const Analog& arMeas, size_t aIndex);

	/**
	* Update or receive a Counter measurement, must have transaction started
	* @param arMeas measurement to be processed
	* @param aIndex index of the measurement
	*/
	void Update(const Counter& arMeas, size_t aIndex);

	/**
	* Update or receive a ControlStatus measurement, must have transaction started
	* @param arMeas measurement to be processed
	* @param aIndex index of the measurement
	*/
	void Update(const ControlStatus& arMeas, size_t aIndex);

	/**
	* Update or receive a SetpointStatus measurement, must have transaction started
	* @param arMeas measurement to be processed
	* @param aIndex index of the measurement
	*/
	void Update(const SetpointStatus& arMeas, size_t aIndex);

protected:

	//concrete class will implement these
	virtual void _Update(const Binary& arPoint, size_t) = 0;
	virtual void _Update(const Analog& arPoint, size_t) = 0;
	virtual void _Update(const Counter& arPoint, size_t) = 0;
	virtual void _Update(const ControlStatus& arPoint, size_t) = 0;
	virtual void _Update(const SetpointStatus& arPoint, size_t) = 0;


};

//Inline the simple public interface functions
inline void IDataObserver::Update(const Binary& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const Analog& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const Counter& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const ControlStatus& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const SetpointStatus& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}

}

#endif

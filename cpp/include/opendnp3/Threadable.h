
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
#ifndef __THREADABLE_H_
#define __THREADABLE_H_

#include <string>

namespace opendnp3
{

/** Objects inherited from this class will implement a Run()
	function.  The Run function will periodically check for an exit request. Users
	will be able to Start() and RequestStop().
*/
class Threadable
{
public:

	Threadable();
	virtual ~Threadable();

	// /These are provided
	// This way any pre/post conditions of Start() can be enforced by the base class
	void Start();
	void RequestStop();

	//allows a client to see if the thread is running
	inline bool IsRunning() {
		return mIsRunning;
	}

	//allows a client to see fi the thread has been "canceled"
	inline bool IsExitRequested() {
		return mIsExitRequested;
	}

	void ResetStopRequest();

protected:



	// allows inherited classes to override and do something to wake the thread
	virtual void SignalStop();

private:
	// implemented Run() should check for this condition
	// and shutdown when requested to do so
	bool mIsExitRequested;

	bool mIsRunning;


	// inherited objects must implement a run function
	virtual void Run() = 0;

};


} //end namespace

#endif

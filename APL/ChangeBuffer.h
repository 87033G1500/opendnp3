//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __CHANGE_BUFFER_H_
#define __CHANGE_BUFFER_H_

#include "DataTypes.h"
#include "DataInterfaces.h"
#include "INotifier.h"
#include "SubjectBase.h"

#include <queue>
#include <mutex>

namespace apl
{

/** Moves measurement data across thread boundaries.
*/
class ChangeBuffer : public IDataObserver, public SubjectBase<std::mutex>
{

public:

	ChangeBuffer() : mMidFlush(false) {}

	void _Start();
	void _End();
	void _Update(const Binary& arPoint, size_t aIndex);
	void _Update(const Analog& arPoint, size_t aIndex);
	void _Update(const Counter& arPoint, size_t aIndex);
	void _Update(const ControlStatus& arPoint, size_t aIndex);
	void _Update(const SetpointStatus& arPoint, size_t aIndex);
	size_t FlushUpdates(apl::IDataObserver* apObserver, bool aClear = true);

	void Clear();

private:

	template <class T>
	static void Dispatch(IDataObserver* apObs, T& arMeas, size_t aIndex)
	{
		apObs->Update(arMeas, aIndex);
	}


	void _Clear();

	bool HasChanges();

	// TODO - Evaluate necesscity of this param
	bool mMidFlush;
	
	std::deque<std::function<void (IDataObserver*)>> mChangeQueue;
	std::mutex mMutex;
};

}

#endif

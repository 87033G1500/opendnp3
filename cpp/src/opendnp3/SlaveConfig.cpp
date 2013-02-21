
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

#include <opendnp3/SlaveConfig.h>

#include <opendnp3/DNPConstants.h>

namespace opendnp3
{

EventMaxConfig::EventMaxConfig() :
	mMaxBinaryEvents(1000),
	mMaxAnalogEvents(1000),
	mMaxCounterEvents(1000),
	mMaxVtoEvents(100)
{}

EventMaxConfig::EventMaxConfig(size_t aMaxBinaryEvents, size_t aMaxAnalogEvents, size_t aMaxCounterEvents, size_t aMaxVtoEvents) :
	mMaxBinaryEvents(aMaxBinaryEvents),
	mMaxAnalogEvents(aMaxAnalogEvents),
	mMaxCounterEvents(aMaxCounterEvents),
	mMaxVtoEvents(aMaxVtoEvents)
{}

SlaveConfig::SlaveConfig() :
	mMaxControls(1),
	mDisableUnsol(false),
	mUnsolMask(true, true, true),
	mAllowTimeSync(false),
	mTimeSyncPeriod(10 * 60 * 1000), //every 10 min
	mUnsolPackDelay(200),
	mUnsolRetryDelay(2000),
	mSelectTimeout(5000),
	mMaxFragSize(DEFAULT_FRAG_SIZE),
	mVtoWriterQueueSize(DEFAULT_VTO_WRITER_QUEUE_SIZE),
	mEventMaxConfig(),
	mStaticBinary(GrpVar(1, 2)),
	mStaticAnalog(GrpVar(30, 1)),
	mStaticCounter(GrpVar(20, 1)),
	mStaticSetpointStatus(GrpVar(40, 1)),
	mEventBinary(GrpVar(2, 1)),
	mEventAnalog(GrpVar(32, 1)),
	mEventCounter(GrpVar(22, 1)),
	mEventVto(GrpVar(113, 0))
{}

}

/* vim: set ts=4 sw=4: */

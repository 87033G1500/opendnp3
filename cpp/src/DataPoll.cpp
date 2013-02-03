
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

#include "ITimeSource.h"
#include "Exception.h"
#include "DataPoll.h"
#include "APDU.h"
#include "ResponseLoader.h"
#include "PointClass.h"
#include "VtoReader.h"

namespace opendnp3
{

/* DataPoll - base class */

DataPoll::DataPoll(Logger* apLogger, IDataObserver* apObs, VtoReader* apVtoReader) :
	MasterTaskBase(apLogger),
	mpObs(apObs),
	mpVtoReader(apVtoReader)
{}

TaskResult DataPoll::_OnPartialResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_CONTINUE;
}

TaskResult DataPoll::_OnFinalResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_SUCCESS;
}

void DataPoll::ReadData(const APDU& f)
{
	ResponseLoader loader(mpLogger, mpObs, mpVtoReader);
	HeaderReadIterator hdr = f.BeginRead();
	for ( ; !hdr.IsEnd(); ++hdr) {
		loader.Process(hdr);
	}
}

/* Class Poll */

ClassPoll::ClassPoll(Logger* apLogger, IDataObserver* apObs, VtoReader* apVtoReader) :
	DataPoll(apLogger, apObs, apVtoReader),
	mClassMask(PC_INVALID)
{}

void ClassPoll::Set(int aClassMask)
{
	mClassMask = aClassMask;
}

void ClassPoll::ConfigureRequest(APDU& arAPDU)
{
	if (mClassMask == PC_INVALID) {
		throw InvalidStateException(LOCATION, "Class mask has not been set");
	}

	arAPDU.Set(FC_READ);
	if (mClassMask & PC_CLASS_0) arAPDU.DoPlaceholderWrite(Group60Var1::Inst());
	if (mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if (mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if (mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}


} //end ns

/* vim: set ts=4 sw=4: */

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

#include "SessionKeys.h"

using namespace openpal;

namespace secauth
{
	
	void SessionKeys::SetKeys (
		const SessionKeysView& view
		)
	{
		auto controlDest = controlBuffer.GetWriteBuffer();
		controlKey = view.controlKey.CopyTo(controlDest);

		auto monitorDest = monitorBuffer.GetWriteBuffer();
		monitorKey = view.monitorKey.CopyTo(monitorDest);
	}

	SessionKeysView SessionKeys::GetView() const
	{
		return SessionKeysView(controlKey, monitorKey);
	}

	bool SessionKeys::DeriveFrom(ISecureRandom& rs, const SessionKeySize& size)
	{
		auto dest1 = controlBuffer.GetWriteBuffer(size);
		auto dest2 = monitorBuffer.GetWriteBuffer(size);

		this->controlKey = controlBuffer.ToReadOnly(size);
		this->monitorKey = monitorBuffer.ToReadOnly(size);
		
		return rs.GetSecureRandom(dest1) && rs.GetSecureRandom(dest2);
	}

	void SessionKeys::ClearMemory()
	{
		controlBuffer.GetWriteBuffer().SetAllTo(0);
		monitorBuffer.GetWriteBuffer().SetAllTo(0);
	}

}



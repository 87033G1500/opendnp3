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
#ifndef __OBJECT_DESCRIPTORS_H_
#define __OBJECT_DESCRIPTORS_H_

#include <openpal/BufferWrapper.h>

#include <opendnp3/Uncopyable.h>
#include <opendnp3/DataTypes.h>

#include "objects/Group1.h"

namespace opendnp3
{
	template <class Desc>
	struct Compose : private PureStatic
	{
		static typename Desc::Target Read(openpal::ReadOnlyBuffer& buff)
		{
			typename Desc::Underlying a = Desc::Underlying::Read(buff);
			return Desc::Convert(a);
		}		
	};

	struct Group1Var2Desc : private PureStatic
	{		
		typedef Binary Target;
		typedef Group1Var2 Underlying;
				
		static Binary Convert(const Group1Var2& gv) { return Binary(gv.flags); }	
	};

}

#endif

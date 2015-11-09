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

#include "LoggingHandler.h"

using namespace openpal;

namespace opendnp3
{
	
LoggingHandler::LoggingHandler(openpal::Logger logger_, IDecoderCallbacks& callbacks_) :	
	logger(logger_),
	callbacks(&callbacks_)
{}


IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var1& value, const openpal::RSlice& object) 
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var2& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var5& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var6& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var7& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var9& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var10& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var11& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var12& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var13& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var14& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var15& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group51Var1>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group51Var2>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group120Var3>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& header, const ICollection<Group120Var4>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	return IINField::Empty();
}

}



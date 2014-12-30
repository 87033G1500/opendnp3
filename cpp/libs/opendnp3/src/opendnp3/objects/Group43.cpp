//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group43.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group43Var1 Group43Var1::Read(ReadBufferView& buffer)
{
  Group43Var1 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group43Var1::Write(const Group43Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}


AnalogCommandEvent Group43Var1::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var1::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value);
}

void Group43Var1::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var1::Write(ConvertGroup43Var1::Apply(value), buff);
}


Group43Var2 Group43Var2::Read(ReadBufferView& buffer)
{
  Group43Var2 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group43Var2::Write(const Group43Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}


AnalogCommandEvent Group43Var2::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var2::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value);
}

void Group43Var2::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var2::Write(ConvertGroup43Var2::Apply(value), buff);
}


Group43Var3 Group43Var3::Read(ReadBufferView& buffer)
{
  Group43Var3 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group43Var3::Write(const Group43Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


AnalogCommandEvent Group43Var3::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var3::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value, gv.time);
}

void Group43Var3::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var3::Write(ConvertGroup43Var3::Apply(value), buff);
}


Group43Var4 Group43Var4::Read(ReadBufferView& buffer)
{
  Group43Var4 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group43Var4::Write(const Group43Var4& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


AnalogCommandEvent Group43Var4::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var4::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value, gv.time);
}

void Group43Var4::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var4::Write(ConvertGroup43Var4::Apply(value), buff);
}


Group43Var5 Group43Var5::Read(ReadBufferView& buffer)
{
  Group43Var5 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group43Var5::Write(const Group43Var5& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}


AnalogCommandEvent Group43Var5::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var5::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value);
}

void Group43Var5::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var5::Write(ConvertGroup43Var5::Apply(value), buff);
}


Group43Var6 Group43Var6::Read(ReadBufferView& buffer)
{
  Group43Var6 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  return obj;
}

void Group43Var6::Write(const Group43Var6& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}


AnalogCommandEvent Group43Var6::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var6::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value);
}

void Group43Var6::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var6::Write(ConvertGroup43Var6::Apply(value), buff);
}


Group43Var7 Group43Var7::Read(ReadBufferView& buffer)
{
  Group43Var7 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group43Var7::Write(const Group43Var7& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


AnalogCommandEvent Group43Var7::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var7::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value, gv.time);
}

void Group43Var7::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var7::Write(ConvertGroup43Var7::Apply(value), buff);
}


Group43Var8 Group43Var8::Read(ReadBufferView& buffer)
{
  Group43Var8 obj;
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group43Var8::Write(const Group43Var8& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


AnalogCommandEvent Group43Var8::ReadTarget(ReadBufferView& buff)
{
  auto gv = Group43Var8::Read(buff);
  return AnalogCommandEventFactory::From(gv.status, gv.value, gv.time);
}

void Group43Var8::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  Group43Var8::Write(ConvertGroup43Var8::Apply(value), buff);
}



}

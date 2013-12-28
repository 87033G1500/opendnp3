//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group32.h"

#include "MeasurementFactory.h"
#include "../WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group32Var1::ID(32,1);

Group32Var1 Group32Var1::Read(ReadOnlyBuffer& buffer)
{
  Group32Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group32Var1::Write(const Group32Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}

Analog Group32Var1::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group32Var1::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var1::Apply(value), buff);
}


const GroupVariationID  Group32Var2::ID(32,2);

Group32Var2 Group32Var2::Read(ReadOnlyBuffer& buffer)
{
  Group32Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group32Var2::Write(const Group32Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}

Analog Group32Var2::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group32Var2::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var2::Apply(value), buff);
}


const GroupVariationID  Group32Var3::ID(32,3);

Group32Var3 Group32Var3::Read(ReadOnlyBuffer& buffer)
{
  Group32Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group32Var3::Write(const Group32Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Analog Group32Var3::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value, gv.time);
}

void Group32Var3::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var3::Apply(value), buff);
}


const GroupVariationID  Group32Var4::ID(32,4);

Group32Var4 Group32Var4::Read(ReadOnlyBuffer& buffer)
{
  Group32Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group32Var4::Write(const Group32Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Analog Group32Var4::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value, gv.time);
}

void Group32Var4::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var4::Apply(value), buff);
}


const GroupVariationID  Group32Var5::ID(32,5);

Group32Var5 Group32Var5::Read(ReadOnlyBuffer& buffer)
{
  Group32Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group32Var5::Write(const Group32Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}

Analog Group32Var5::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group32Var5::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var5::Apply(value), buff);
}


const GroupVariationID  Group32Var6::ID(32,6);

Group32Var6 Group32Var6::Read(ReadOnlyBuffer& buffer)
{
  Group32Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  return obj;
}

void Group32Var6::Write(const Group32Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}

Analog Group32Var6::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group32Var6::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var6::Apply(value), buff);
}


const GroupVariationID  Group32Var7::ID(32,7);

Group32Var7 Group32Var7::Read(ReadOnlyBuffer& buffer)
{
  Group32Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group32Var7::Write(const Group32Var7& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Analog Group32Var7::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value, gv.time);
}

void Group32Var7::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var7::Apply(value), buff);
}


const GroupVariationID  Group32Var8::ID(32,8);

Group32Var8 Group32Var8::Read(ReadOnlyBuffer& buffer)
{
  Group32Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group32Var8::Write(const Group32Var8& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Analog Group32Var8::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value, gv.time);
}

void Group32Var8::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup32Var8::Apply(value), buff);
}



}

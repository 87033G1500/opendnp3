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

#include "Group122.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

// ------- Group122Var1 -------

bool Group122Var1::Read(ReadBufferView& buffer, Group122Var1& output)
{
  return Parse::Many(buffer, output.flags, output.assocId, output.value);
}

bool Group122Var1::Write(const Group122Var1& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags, arg.assocId, arg.value);
}

bool Group122Var1::ReadTarget(ReadBufferView& buff, SecurityStat& output)
{
  Group122Var1 value;
  if(Read(buff, value))
  {
    output = SecurityStatFactory::From(value.flags, value.assocId, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group122Var1::WriteTarget(const SecurityStat& value, openpal::WriteBufferView& buff)
{
  return Group122Var1::Write(ConvertGroup122Var1::Apply(value), buff);
}

// ------- Group122Var2 -------

bool Group122Var2::Read(ReadBufferView& buffer, Group122Var2& output)
{
  return Parse::Many(buffer, output.flags, output.assocId, output.value, output.time);
}

bool Group122Var2::Write(const Group122Var2& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags, arg.assocId, arg.value, arg.time);
}

bool Group122Var2::ReadTarget(ReadBufferView& buff, SecurityStat& output)
{
  Group122Var2 value;
  if(Read(buff, value))
  {
    output = SecurityStatFactory::From(value.flags, value.assocId, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group122Var2::WriteTarget(const SecurityStat& value, openpal::WriteBufferView& buff)
{
  return Group122Var2::Write(ConvertGroup122Var2::Apply(value), buff);
}


}

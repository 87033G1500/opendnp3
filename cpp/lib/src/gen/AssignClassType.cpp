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
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "opendnp3/gen/AssignClassType.h"
#include <cstring>
#include <stdexcept>

namespace opendnp3 {

uint8_t AssignClassTypeToType(AssignClassType arg)
{
  return static_cast<uint8_t>(arg);
}
AssignClassType AssignClassTypeFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return AssignClassType::BinaryInput;
    case(0x1):
      return AssignClassType::DoubleBinaryInput;
    case(0x2):
      return AssignClassType::Counter;
    case(0x3):
      return AssignClassType::FrozenCounter;
    case(0x4):
      return AssignClassType::AnalogInput;
    case(0x5):
      return AssignClassType::BinaryOutputStatus;
    case(0x6):
      return AssignClassType::AnalogOutputStatus;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}
char const* AssignClassTypeToString(AssignClassType arg)
{
  switch(arg)
  {
    case(AssignClassType::BinaryInput):
      return "BinaryInput";
    case(AssignClassType::DoubleBinaryInput):
      return "DoubleBinaryInput";
    case(AssignClassType::Counter):
      return "Counter";
    case(AssignClassType::FrozenCounter):
      return "FrozenCounter";
    case(AssignClassType::AnalogInput):
      return "AnalogInput";
    case(AssignClassType::BinaryOutputStatus):
      return "BinaryOutputStatus";
    case(AssignClassType::AnalogOutputStatus):
      return "AnalogOutputStatus";
    default:
      return "UNDEFINED";
  }
}
AssignClassType AssignClassTypeFromString(char const* arg)
{
  if(std::strncmp(arg, "BinaryInput", 11)) return AssignClassType::BinaryInput;
  if(std::strncmp(arg, "DoubleBinaryInput", 17)) return AssignClassType::DoubleBinaryInput;
  if(std::strncmp(arg, "Counter", 7)) return AssignClassType::Counter;
  if(std::strncmp(arg, "FrozenCounter", 13)) return AssignClassType::FrozenCounter;
  if(std::strncmp(arg, "AnalogInput", 11)) return AssignClassType::AnalogInput;
  if(std::strncmp(arg, "BinaryOutputStatus", 18)) return AssignClassType::BinaryOutputStatus;
  if(std::strncmp(arg, "AnalogOutputStatus", 18)) return AssignClassType::AnalogOutputStatus;
  else throw new std::invalid_argument(std::string("Unknown value: ") + arg);
}

}

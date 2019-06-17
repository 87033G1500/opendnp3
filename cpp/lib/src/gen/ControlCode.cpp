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

#include "opendnp3/gen/ControlCode.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t ControlCodeSpec::to_type(ControlCode arg)
{
  return static_cast<uint8_t>(arg);
}

ControlCode ControlCodeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return ControlCode::NUL;
    case(0x20):
      return ControlCode::NUL_CANCEL;
    case(0x1):
      return ControlCode::PULSE_ON;
    case(0x21):
      return ControlCode::PULSE_ON_CANCEL;
    case(0x2):
      return ControlCode::PULSE_OFF;
    case(0x22):
      return ControlCode::PULSE_OFF_CANCEL;
    case(0x3):
      return ControlCode::LATCH_ON;
    case(0x23):
      return ControlCode::LATCH_ON_CANCEL;
    case(0x4):
      return ControlCode::LATCH_OFF;
    case(0x24):
      return ControlCode::LATCH_OFF_CANCEL;
    case(0x41):
      return ControlCode::CLOSE_PULSE_ON;
    case(0x61):
      return ControlCode::CLOSE_PULSE_ON_CANCEL;
    case(0x81):
      return ControlCode::TRIP_PULSE_ON;
    case(0xA1):
      return ControlCode::TRIP_PULSE_ON_CANCEL;
    default:
      return ControlCode::UNDEFINED;
  }
}

char const* ControlCodeSpec::to_string(ControlCode arg)
{
  switch(arg)
  {
    case(ControlCode::NUL):
      return "NUL";
    case(ControlCode::NUL_CANCEL):
      return "NUL_CANCEL";
    case(ControlCode::PULSE_ON):
      return "PULSE_ON";
    case(ControlCode::PULSE_ON_CANCEL):
      return "PULSE_ON_CANCEL";
    case(ControlCode::PULSE_OFF):
      return "PULSE_OFF";
    case(ControlCode::PULSE_OFF_CANCEL):
      return "PULSE_OFF_CANCEL";
    case(ControlCode::LATCH_ON):
      return "LATCH_ON";
    case(ControlCode::LATCH_ON_CANCEL):
      return "LATCH_ON_CANCEL";
    case(ControlCode::LATCH_OFF):
      return "LATCH_OFF";
    case(ControlCode::LATCH_OFF_CANCEL):
      return "LATCH_OFF_CANCEL";
    case(ControlCode::CLOSE_PULSE_ON):
      return "CLOSE_PULSE_ON";
    case(ControlCode::CLOSE_PULSE_ON_CANCEL):
      return "CLOSE_PULSE_ON_CANCEL";
    case(ControlCode::TRIP_PULSE_ON):
      return "TRIP_PULSE_ON";
    case(ControlCode::TRIP_PULSE_ON_CANCEL):
      return "TRIP_PULSE_ON_CANCEL";
    default:
      return "UNDEFINED";
  }
}

char const* ControlCodeSpec::to_human_string(ControlCode arg)
{
  switch(arg)
  {
    case(ControlCode::NUL):
      return "NUL";
    case(ControlCode::NUL_CANCEL):
      return "NUL_CANCEL";
    case(ControlCode::PULSE_ON):
      return "PULSE_ON";
    case(ControlCode::PULSE_ON_CANCEL):
      return "PULSE_ON_CANCEL";
    case(ControlCode::PULSE_OFF):
      return "PULSE_OFF";
    case(ControlCode::PULSE_OFF_CANCEL):
      return "PULSE_OFF_CANCEL";
    case(ControlCode::LATCH_ON):
      return "LATCH_ON";
    case(ControlCode::LATCH_ON_CANCEL):
      return "LATCH_ON_CANCEL";
    case(ControlCode::LATCH_OFF):
      return "LATCH_OFF";
    case(ControlCode::LATCH_OFF_CANCEL):
      return "LATCH_OFF_CANCEL";
    case(ControlCode::CLOSE_PULSE_ON):
      return "CLOSE_PULSE_ON";
    case(ControlCode::CLOSE_PULSE_ON_CANCEL):
      return "CLOSE_PULSE_ON_CANCEL";
    case(ControlCode::TRIP_PULSE_ON):
      return "TRIP_PULSE_ON";
    case(ControlCode::TRIP_PULSE_ON_CANCEL):
      return "TRIP_PULSE_ON_CANCEL";
    default:
      return "UNDEFINED";
  }
}

ControlCode ControlCodeSpec::from_string(const std::string& arg)
{
  if(arg == "NUL") return ControlCode::NUL;
  if(arg == "NUL_CANCEL") return ControlCode::NUL_CANCEL;
  if(arg == "PULSE_ON") return ControlCode::PULSE_ON;
  if(arg == "PULSE_ON_CANCEL") return ControlCode::PULSE_ON_CANCEL;
  if(arg == "PULSE_OFF") return ControlCode::PULSE_OFF;
  if(arg == "PULSE_OFF_CANCEL") return ControlCode::PULSE_OFF_CANCEL;
  if(arg == "LATCH_ON") return ControlCode::LATCH_ON;
  if(arg == "LATCH_ON_CANCEL") return ControlCode::LATCH_ON_CANCEL;
  if(arg == "LATCH_OFF") return ControlCode::LATCH_OFF;
  if(arg == "LATCH_OFF_CANCEL") return ControlCode::LATCH_OFF_CANCEL;
  if(arg == "CLOSE_PULSE_ON") return ControlCode::CLOSE_PULSE_ON;
  if(arg == "CLOSE_PULSE_ON_CANCEL") return ControlCode::CLOSE_PULSE_ON_CANCEL;
  if(arg == "TRIP_PULSE_ON") return ControlCode::TRIP_PULSE_ON;
  if(arg == "TRIP_PULSE_ON_CANCEL") return ControlCode::TRIP_PULSE_ON_CANCEL;
  else return ControlCode::UNDEFINED;
}


}

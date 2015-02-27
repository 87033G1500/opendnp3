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

#include "Group120.h"

#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group120Var3::Read(ReadBufferView& buffer, Group120Var3& output)
{
  return Parse::Many(buffer, output.challengeSeqNum, output.userNum);
}

void Group120Var3::Write(const Group120Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt32::Write(buffer, arg.challengeSeqNum);
  buffer.Advance(4);
  UInt16::Write(buffer, arg.userNum);
  buffer.Advance(2);
}


bool Group120Var4::Read(ReadBufferView& buffer, Group120Var4& output)
{
  return Parse::Many(buffer, output.userNum);
}

void Group120Var4::Write(const Group120Var4& arg, openpal::WriteBufferView& buffer)
{
  UInt16::Write(buffer, arg.userNum);
  buffer.Advance(2);
}



}

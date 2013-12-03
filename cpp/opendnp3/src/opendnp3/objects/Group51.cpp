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

#include "Group51.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group51Var1 Group51Var1::Read(ReadOnlyBuffer& buffer)
{
  Group51Var1 obj;
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group51Var2 Group51Var2::Read(ReadOnlyBuffer& buffer)
{
  Group51Var2 obj;
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}


}

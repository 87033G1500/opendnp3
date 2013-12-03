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

#include <openpal/BufferWrapper.h>

namespace opendnp3 {

struct Group40Var1
{
  static Group40Var1 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  int32_t value;
};

struct Group40Var2
{
  static Group40Var2 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  int16_t value;
};

struct Group40Var3
{
  static Group40Var3 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  float value;
};

struct Group40Var4
{
  static Group40Var4 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  float value;
};


}

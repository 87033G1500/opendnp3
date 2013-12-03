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

struct Group23Var1
{
  static Group23Var1 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group23Var2
{
  static Group23Var2 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group23Var3
{
  static Group23Var3 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group23Var4
{
  static Group23Var4 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group23Var5
{
  static Group23Var5 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group23Var6
{
  static Group23Var6 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};

struct Group23Var7
{
  static Group23Var7 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group23Var8
{
  static Group23Var8 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};


}

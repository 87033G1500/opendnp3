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

#ifndef OPENDNP3_GROUP41_H
#define OPENDNP3_GROUP41_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3 {

// Analog Output - Any Variation
struct Group41Var0
{
  static GroupVariationID ID() { return GroupVariationID(41,0); }
};

// Analog Output - 32-bit With Flag
struct Group41Var1
{
  static GroupVariationID ID() { return GroupVariationID(41,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group41Var1&);
  static bool Write(const Group41Var1&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  int32_t value;
  uint8_t status;

  typedef AnalogOutputInt32 Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputInt32&);
  static bool WriteTarget(const AnalogOutputInt32&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputInt32> Inst() { return DNP3Serializer<AnalogOutputInt32>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - 16-bit With Flag
struct Group41Var2
{
  static GroupVariationID ID() { return GroupVariationID(41,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group41Var2&);
  static bool Write(const Group41Var2&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  int16_t value;
  uint8_t status;

  typedef AnalogOutputInt16 Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputInt16&);
  static bool WriteTarget(const AnalogOutputInt16&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputInt16> Inst() { return DNP3Serializer<AnalogOutputInt16>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - Single-precision
struct Group41Var3
{
  static GroupVariationID ID() { return GroupVariationID(41,3); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group41Var3&);
  static bool Write(const Group41Var3&, openpal::WriteBufferView&);

  typedef float ValueType;
  float value;
  uint8_t status;

  typedef AnalogOutputFloat32 Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputFloat32&);
  static bool WriteTarget(const AnalogOutputFloat32&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputFloat32> Inst() { return DNP3Serializer<AnalogOutputFloat32>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - Double-precision
struct Group41Var4
{
  static GroupVariationID ID() { return GroupVariationID(41,4); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group41Var4&);
  static bool Write(const Group41Var4&, openpal::WriteBufferView&);

  typedef double ValueType;
  double value;
  uint8_t status;

  typedef AnalogOutputDouble64 Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputDouble64&);
  static bool WriteTarget(const AnalogOutputDouble64&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputDouble64> Inst() { return DNP3Serializer<AnalogOutputDouble64>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif

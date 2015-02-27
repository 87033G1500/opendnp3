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

#ifndef OPENDNP3_GROUP30_H
#define OPENDNP3_GROUP30_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group30Var1
{
  static GroupVariationID ID() { return GroupVariationID(30,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group30Var1&);
  static void Write(const Group30Var1&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group30Var2
{
  static GroupVariationID ID() { return GroupVariationID(30,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group30Var2&);
  static void Write(const Group30Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group30Var3
{
  static GroupVariationID ID() { return GroupVariationID(30,3); }
  static uint32_t Size() { return 4; }
  static bool Read(openpal::ReadBufferView&, Group30Var3&);
  static void Write(const Group30Var3&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  int32_t value;
};

struct Group30Var4
{
  static GroupVariationID ID() { return GroupVariationID(30,4); }
  static uint32_t Size() { return 2; }
  static bool Read(openpal::ReadBufferView&, Group30Var4&);
  static void Write(const Group30Var4&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  int16_t value;
};

struct Group30Var5
{
  static GroupVariationID ID() { return GroupVariationID(30,5); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group30Var5&);
  static void Write(const Group30Var5&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group30Var6
{
  static GroupVariationID ID() { return GroupVariationID(30,6); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group30Var6&);
  static void Write(const Group30Var6&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static void WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};


}

#endif

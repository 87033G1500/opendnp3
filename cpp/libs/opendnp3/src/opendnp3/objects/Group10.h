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

#ifndef OPENDNP3_GROUP10_H
#define OPENDNP3_GROUP10_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group10Var0
{
  static GroupVariationID ID() { return GroupVariationID(10,0); }
};

struct Group10Var1
{
  static GroupVariationID ID() { return GroupVariationID(10,1); }
};

struct Group10Var2
{
  static GroupVariationID ID() { return GroupVariationID(10,2); }
  static uint32_t Size() { return 1; }
  static bool Read(openpal::ReadBufferView&, Group10Var2&);
  static bool Write(const Group10Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef BinaryOutputStatus Target;
  static bool ReadTarget(openpal::ReadBufferView&, BinaryOutputStatus&);
  static bool WriteTarget(const BinaryOutputStatus&, openpal::WriteBufferView&);

  uint8_t flags;
};


}

#endif

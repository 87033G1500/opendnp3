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

#include "opendnp3/gen/Attributes.h"

namespace opendnp3 {

MeasAttr GetMeasAttr(GroupVariation gv)
{
  switch(gv)
  {
    case(GroupVariation::Group1Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group2Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group2Var2):
        return MeasAttr(true, true);
    case(GroupVariation::Group2Var3):
        return MeasAttr(false, true);
    case(GroupVariation::Group3Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group4Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group4Var2):
        return MeasAttr(true, true);
    case(GroupVariation::Group4Var3):
        return MeasAttr(false, true);
    case(GroupVariation::Group10Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group11Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group11Var2):
        return MeasAttr(true, true);
    case(GroupVariation::Group13Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group13Var2):
        return MeasAttr(true, true);
    case(GroupVariation::Group20Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group20Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group21Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group21Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group21Var5):
        return MeasAttr(true, true);
    case(GroupVariation::Group21Var6):
        return MeasAttr(true, true);
    case(GroupVariation::Group22Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group22Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group22Var5):
        return MeasAttr(true, true);
    case(GroupVariation::Group22Var6):
        return MeasAttr(true, true);
    case(GroupVariation::Group23Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group23Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group23Var5):
        return MeasAttr(true, true);
    case(GroupVariation::Group23Var6):
        return MeasAttr(true, true);
    case(GroupVariation::Group30Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group30Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group30Var5):
        return MeasAttr(false, true);
    case(GroupVariation::Group30Var6):
        return MeasAttr(false, true);
    case(GroupVariation::Group32Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group32Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group32Var3):
        return MeasAttr(true, true);
    case(GroupVariation::Group32Var4):
        return MeasAttr(true, true);
    case(GroupVariation::Group32Var5):
        return MeasAttr(false, true);
    case(GroupVariation::Group32Var6):
        return MeasAttr(false, true);
    case(GroupVariation::Group32Var7):
        return MeasAttr(true, true);
    case(GroupVariation::Group32Var8):
        return MeasAttr(true, true);
    case(GroupVariation::Group40Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group40Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group40Var3):
        return MeasAttr(false, true);
    case(GroupVariation::Group40Var4):
        return MeasAttr(false, true);
    case(GroupVariation::Group42Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group42Var2):
        return MeasAttr(false, true);
    case(GroupVariation::Group42Var3):
        return MeasAttr(true, true);
    case(GroupVariation::Group42Var4):
        return MeasAttr(true, true);
    case(GroupVariation::Group42Var5):
        return MeasAttr(false, true);
    case(GroupVariation::Group42Var6):
        return MeasAttr(false, true);
    case(GroupVariation::Group42Var7):
        return MeasAttr(true, true);
    case(GroupVariation::Group42Var8):
        return MeasAttr(true, true);
    case(GroupVariation::Group43Var3):
        return MeasAttr(true, false);
    case(GroupVariation::Group43Var4):
        return MeasAttr(true, false);
    case(GroupVariation::Group43Var7):
        return MeasAttr(true, false);
    case(GroupVariation::Group43Var8):
        return MeasAttr(true, false);
    case(GroupVariation::Group50Var1):
        return MeasAttr(true, false);
    case(GroupVariation::Group50Var4):
        return MeasAttr(true, false);
    case(GroupVariation::Group51Var1):
        return MeasAttr(true, false);
    case(GroupVariation::Group51Var2):
        return MeasAttr(true, false);
    case(GroupVariation::Group121Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group122Var1):
        return MeasAttr(false, true);
    case(GroupVariation::Group122Var2):
        return MeasAttr(true, true);
    default:
      return MeasAttr(false, false);
  }
}

}

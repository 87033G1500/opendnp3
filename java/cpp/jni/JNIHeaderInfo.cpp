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
// Copyright 2016 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "JNIHeaderInfo.h"

#include "JNI.h"
#include <assert.h>

namespace jni
{
    bool HeaderInfo::init(JNIEnv* env)
    {

        this->clazz = env->FindClass("Lcom/automatak/dnp3/HeaderInfo;");
        if(!this->clazz) return false;

        this->init6Constructor = env->GetMethodID(this->clazz, "<init>", "(Lcom/automatak/dnp3/enums/GroupVariation;Lcom/automatak/dnp3/enums/QualifierCode;Lcom/automatak/dnp3/enums/TimestampMode;ZZI)V");
        if(!this->init6Constructor) return false;

        return true;
    }

    jobject HeaderInfo::init6(JNIEnv* env, jobject arg0, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5)
    {
        return env->NewObject(this->clazz, this->init6Constructor, arg0, arg1, arg2, arg3, arg4, arg5);
    }
}

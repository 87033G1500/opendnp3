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

#include "JNIBinaryInput.h"

#include "JNI.h"
#include <assert.h>

namespace jni
{
    bool BinaryInput::init(JNIEnv* env)
    {

        this->clazz = env->FindClass("Lcom/automatak/dnp3/BinaryInput;");
        if(!this->clazz) return false;

        this->init3Constructor = env->GetMethodID(this->clazz, "<init>", "(ZBJ)V");
        if(!this->init3Constructor) return false;

        return true;
    }

    jobject BinaryInput::init3(JNIEnv* env, jobject arg0, jobject arg1, jobject arg2)
    {
        return env->NewObject(this->clazz, this->init3Constructor, arg0, arg1, arg2);
    }
}

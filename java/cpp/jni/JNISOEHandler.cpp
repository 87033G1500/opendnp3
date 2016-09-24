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

#include "JNISOEHandler.h"

#include "JNI.h"
#include <assert.h>

namespace jni
{
    bool SOEHandler::init(JNIEnv* env)
    {

        this->clazz = env->FindClass("Lcom/automatak/dnp3/SOEHandler;");
        if(!this->clazz) return false;

        this->startMethod = env->GetMethodID(this->clazz, "start", "()V");
        if(!this->startMethod) return false;

        this->endMethod = env->GetMethodID(this->clazz, "end", "()V");
        if(!this->endMethod) return false;

        this->processAIMethod = env->GetMethodID(this->clazz, "processAI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processAIMethod) return false;

        this->processFCMethod = env->GetMethodID(this->clazz, "processFC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processFCMethod) return false;

        this->processAOSMethod = env->GetMethodID(this->clazz, "processAOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processAOSMethod) return false;

        this->processCMethod = env->GetMethodID(this->clazz, "processC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processCMethod) return false;

        this->processDBIMethod = env->GetMethodID(this->clazz, "processDBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processDBIMethod) return false;

        this->processBOSMethod = env->GetMethodID(this->clazz, "processBOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processBOSMethod) return false;

        this->processBIMethod = env->GetMethodID(this->clazz, "processBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
        if(!this->processBIMethod) return false;

        return true;
    }
}


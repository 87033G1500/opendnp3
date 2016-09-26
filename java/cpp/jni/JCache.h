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

#ifndef OPENDNP3JAVA_OPENDNP3_JNITYPES_H
#define OPENDNP3JAVA_OPENDNP3_JNITYPES_H

#include "openpal/util/Uncopyable.h"

#include "JNIMasterStackConfig.h"
#include "JNIMasterConfig.h"
#include "JNILinkLayerConfig.h"
#include "JNILogEntry.h"
#include "JNILogHandler.h"
#include "JNIClassField.h"
#include "JNISOEHandler.h"
#include "JNIHeaderInfo.h"
#include "JNIGroupVariation.h"
#include "JNIQualifierCode.h"
#include "JNITimestampMode.h"
#include "JNIIndexedValue.h"
#include "JNIBinaryInput.h"
#include "JNIDoubleBitBinaryInput.h"
#include "JNIAnalogInput.h"
#include "JNICounter.h"
#include "JNIFrozenCounter.h"
#include "JNIBinaryOutputStatus.h"
#include "JNIAnalogOutputStatus.h"
#include "JNIDuration.h"
#include "JNIArrayList.h"
#include "JNIDoubleBit.h"
#include "JNITimeSyncMode.h"
#include "JNIMasterApplication.h"
#include "JNIIINField.h"
#include "JNIMasterTaskType.h"
#include "JNITaskId.h"
#include "JNITaskInfo.h"
#include "JNITaskCompletion.h"
#include "JNIIterable.h"
#include "JNIIterator.h"
#include "JNIClassAssignment.h"
#include "JNIRange.h"
#include "JNICommandProcessor.h"
#include "JNIStack.h"
#include "JNICommandBuilderImpl.h"
#include "JNICommandHeaders.h"
#include "JNICommandTaskResult.h"
#include "JNICommandPointResult.h"
#include "JNICommandPointState.h"
#include "JNICommandStatus.h"
#include "JNICompletableFuture.h"
#include "JNIControlRelayOutputBlock.h"
#include "JNIAnalogOutputInt16.h"
#include "JNIAnalogOutputInt32.h"
#include "JNIAnalogOutputFloat32.h"
#include "JNIAnalogOutputDouble64.h"
#include "JNIControlCode.h"

namespace jni
{
    struct JCache: private openpal::StaticOnly
    {
        static bool init(JNIEnv* env);
        static void cleanup(JNIEnv* env);

        static jni::MasterStackConfig MasterStackConfig;
        static jni::MasterConfig MasterConfig;
        static jni::LinkLayerConfig LinkLayerConfig;
        static jni::LogEntry LogEntry;
        static jni::LogHandler LogHandler;
        static jni::ClassField ClassField;
        static jni::SOEHandler SOEHandler;
        static jni::HeaderInfo HeaderInfo;
        static jni::GroupVariation GroupVariation;
        static jni::QualifierCode QualifierCode;
        static jni::TimestampMode TimestampMode;
        static jni::IndexedValue IndexedValue;
        static jni::BinaryInput BinaryInput;
        static jni::DoubleBitBinaryInput DoubleBitBinaryInput;
        static jni::AnalogInput AnalogInput;
        static jni::Counter Counter;
        static jni::FrozenCounter FrozenCounter;
        static jni::BinaryOutputStatus BinaryOutputStatus;
        static jni::AnalogOutputStatus AnalogOutputStatus;
        static jni::Duration Duration;
        static jni::ArrayList ArrayList;
        static jni::DoubleBit DoubleBit;
        static jni::TimeSyncMode TimeSyncMode;
        static jni::MasterApplication MasterApplication;
        static jni::IINField IINField;
        static jni::MasterTaskType MasterTaskType;
        static jni::TaskId TaskId;
        static jni::TaskInfo TaskInfo;
        static jni::TaskCompletion TaskCompletion;
        static jni::Iterable Iterable;
        static jni::Iterator Iterator;
        static jni::ClassAssignment ClassAssignment;
        static jni::Range Range;
        static jni::CommandProcessor CommandProcessor;
        static jni::Stack Stack;
        static jni::CommandBuilderImpl CommandBuilderImpl;
        static jni::CommandHeaders CommandHeaders;
        static jni::CommandTaskResult CommandTaskResult;
        static jni::CommandPointResult CommandPointResult;
        static jni::CommandPointState CommandPointState;
        static jni::CommandStatus CommandStatus;
        static jni::CompletableFuture CompletableFuture;
        static jni::ControlRelayOutputBlock ControlRelayOutputBlock;
        static jni::AnalogOutputInt16 AnalogOutputInt16;
        static jni::AnalogOutputInt32 AnalogOutputInt32;
        static jni::AnalogOutputFloat32 AnalogOutputFloat32;
        static jni::AnalogOutputDouble64 AnalogOutputDouble64;
        static jni::ControlCode ControlCode;
    };
}

#endif

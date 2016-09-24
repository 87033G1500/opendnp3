/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "SOEHandlerAdapter.h"

#include "JNI.h"
#include "JNIStrings.h"

using namespace opendnp3;

void SOEHandlerAdapter::Start()
{
	const auto env = JNI::GetEnv();

	// cache all the method ids
	if (!this->initialized)
	{
		using namespace classes::SOEHandler;

		this->startMethod = JNI::GetMethod(env, proxy, methods::start);
		this->endMethod = JNI::GetMethod(env, proxy, methods::end);
		
		// header info stuff		
		this->headerInfoConstructor = JNI::GetMethod(env, classes::HeaderInfo::fqcn, classes::HeaderInfo::constructors::init6);		
		this->gvFromType = JNI::GetStaticMethod(env, classes::GroupVariation::fqcn, classes::GroupVariation::methods::fromType);
		this->qualiferCodefromType = JNI::GetStaticMethod(env, classes::QualifierCode::fqcn, classes::GroupVariation::methods::fromType);
		this->timestampModefromType = JNI::GetStaticMethod(env, classes::TimestampMode::fqcn, classes::TimestampMode::methods::fromType);

		// measurement process methods
		this->processBIMethod = JNI::GetMethod(env, proxy, methods::processBI);
		this->processDBIMethod = JNI::GetMethod(env, proxy, methods::processDBI);
		this->processAIMethod = JNI::GetMethod(env, proxy, methods::processAI);
		this->processCMethod = JNI::GetMethod(env, proxy, methods::processC);
		this->processFCMethod = JNI::GetMethod(env, proxy, methods::processFC);
		this->processBOSMethod = JNI::GetMethod(env, proxy, methods::processBOS);
		this->processAOSMethod = JNI::GetMethod(env, proxy, methods::processAOS);

		/*
		// measurement stuff
		this->binaryConstructor = JNI::GetMethod(env, classes::BinaryInput::fqcn, classes::BinaryInput::constructors::init3);
		this->doubleBinaryConstructor = JNI::GetMethod(env, classes::DoubleBitBinaryInput::fqcn, classes::DoubleBitBinaryInput::constructors::init3);
		this->analogConstructor = JNI::GetMethod(env, classes::AnalogInput::fqcn, classes::AnalogInput::constructors::init3);
		this->counterConstructor = JNI::GetMethod(env, classes::Counter::fqcn, classes::Counter::constructors::init3);
		this->frozenCounterConstructor = JNI::GetMethod(env, classes::FrozenCounter::fqcn, classes::FrozenCounter::constructors::init3);
		this->boStatusConstructor = JNI::GetMethod(env, classes::BinaryOutputStatus::fqcn, classes::BinaryOutputStatus::constructors::init3);
		this->aoStatusConstructor = JNI::GetMethod(env, classes::AnalogOutputStatus::fqcn, classes::AnalogOutputStatus::constructors::init3);
		this->indexedValueConstructor = JNI::GetMethod(env, classes::IndexedValue::fqcn, classes::IndexedValue::constructors::init2);
		*/

		this->initialized = true;
	}

	env->CallVoidMethod(proxy, this->startMethod);
}

void SOEHandlerAdapter::End()
{
	const auto env = JNI::GetEnv();
	env->CallVoidMethod(proxy, this->endMethod);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
	const auto env = JNI::GetEnv();

	
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	const auto env = JNI::GetEnv();
}

jobject SOEHandlerAdapter::Convert(JNIEnv* env, const opendnp3::HeaderInfo& info)
{	
	jint gvRaw = GroupVariationToType(info.gv);
	jint qcRaw = QualifierCodeToType(info.qualifier);
	jint tsModeRaw = static_cast<int>(info.tsmode);

	auto gv = env->CallStaticObjectMethod(this->gvFromType.clazz, this->gvFromType.method, gvRaw);
	auto qc = env->CallStaticObjectMethod(this->qualiferCodefromType.clazz, this->qualiferCodefromType.method, qcRaw);
	auto tsmode = env->CallStaticObjectMethod(this->timestampModefromType.clazz, this->timestampModefromType.method, tsModeRaw);
	jboolean isEvent = info.isEventVariation;
	jboolean flagsValid = info.flagsValid;
	jint headerIndex = info.headerIndex;

	return env->NewObject(this->headerInfoConstructor.clazz, this->headerInfoConstructor.method, gv, qc, tsmode, isEvent, flagsValid, headerIndex);
}


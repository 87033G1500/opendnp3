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
#ifndef OPENDNP3_SOEHANDLERADAPTER_H
#define OPENDNP3_SOEHANDLERADAPTER_H

#include <jni.h>

#include <opendnp3/master/ISOEHandler.h>

#include "GlobalRef.h"

class SOEHandlerAdapter : public opendnp3::ISOEHandler
{
public:

	SOEHandlerAdapter(jobject proxy) : proxy(proxy) {}

	virtual void Start() override;
	virtual void End() override;

	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) override;
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) override {}
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) override {}
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) override {}
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) override {}
	virtual void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::SecurityStat>>& values) override {}
	

private:	

	jobject Convert(JNIEnv* env, const opendnp3::HeaderInfo& info);

	GlobalRef proxy;
		
	// cached JNI initialized on first usage ids
	bool initialized = false;

	jmethodID startMethod = nullptr;
	jmethodID endMethod = nullptr;

	jmethodID processBIMethod = nullptr;
	jmethodID processDBIMethod = nullptr;
	jmethodID processAIMethod = nullptr;
	jmethodID processCMethod = nullptr;
	jmethodID processFCMethod = nullptr;
	jmethodID processBOSMethod = nullptr;
	jmethodID processAOSMethod = nullptr;
	jmethodID processOSMethod = nullptr;

	// cached header info stuff
	ClassMethodPair headerInfoConstructor;
	ClassMethodPair gvFromType;	
	ClassMethodPair qualiferCodefromType;
	ClassMethodPair timestampModefromType;

	// cached measurement type stuff
	ClassMethodPair binaryConstructor;
	ClassMethodPair doubleBinaryConstructor;
	ClassMethodPair analogConstructor;
	ClassMethodPair counterConstructor;
	ClassMethodPair frozenCounterConstructor;
	ClassMethodPair boStatusConstructor;
	ClassMethodPair aoStatusConstructor;

	ClassMethodPair indexedValueConstructor;
};

#endif

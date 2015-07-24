/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef SECAUTH_AUTHREQUESTHANDLER_H
#define SECAUTH_AUTHREQUESTHANDLER_H

#include <opendnp3/app/parsing/IAPDUHandler.h>
#include <opendnp3/outstation/OutstationContext.h>

#include "IAuthRequestHandler.h"

namespace secauth
{

class AuthRequestHandler : public opendnp3::IAPDUHandler, private openpal::Uncopyable
{
	public:

		AuthRequestHandler(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, IAuthRequestHandler& handler, openpal::Logger logger);

		virtual opendnp3::IINField ProcessHeader(const opendnp3::CountHeader& header, const opendnp3::ICollection<opendnp3::Group120Var4>& values) override final;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var1& value) override final;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var2& value) override final;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var6& value) override final;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var10& value) override final;

		virtual bool IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc) override final;

		

	private:
		openpal::Logger logger;
		openpal::ReadBufferView fragment;
		opendnp3::APDUHeader apduheader;		
		IAuthRequestHandler* pHandler;
};


}

#endif


/*
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
#include "openpal/logging/LogRoot.h"

#include "openpal/logging/LogEntry.h"
#include "openpal/logging/LogMacros.h"
#include "openpal/logging/ILogHandler.h"
#include "openpal/logging/LogMacros.h"

#include "openpal/logging/StringFormatting.h"

namespace openpal
{

LogRoot::LogRoot(ILogHandler* gandler, char const* alias, LogFilters filters) :
	m_handler(gandler),
	m_filters(filters),
	m_alias(AllocateCopy(alias))
{

}

LogRoot::LogRoot(const LogRoot& copy, char const* alias) :
	m_handler(copy.m_handler),
	m_filters(copy.m_filters),
	m_alias(AllocateCopy(alias))
{

}

LogRoot::LogRoot(const LogRoot& copy, char const* alias, LogFilters filters) :
	m_handler(copy.m_handler),
	m_filters(filters),
	m_alias(AllocateCopy(alias))
{

}

LogRoot::~LogRoot()
{
	delete[] m_alias;
}

void LogRoot::Log(const LogFilters& filters, char const* location, char const* message, int errorCode)
{
	if (m_handler)
	{
		LogEntry le(m_alias, filters, location, message, errorCode);
		m_handler->Log(le);
	}
}

Logger LogRoot::GetLogger()
{
	return Logger(this);
}

bool LogRoot::IsEnabled(const LogFilters& rhs) const
{
	return m_handler && (this->m_filters & rhs);
}

bool LogRoot::HasAny(const LogFilters& rhs) const
{
	return this->m_filters & rhs;
}

void LogRoot::SetFilters(const LogFilters& filters)
{
	m_filters = filters;
}

const LogFilters& LogRoot::GetFilters() const
{
	return m_filters;
}

}

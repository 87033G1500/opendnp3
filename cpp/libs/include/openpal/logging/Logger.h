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
#ifndef OPENPAL_LOGGER_H
#define OPENPAL_LOGGER_H

#include "openpal/logging/ILogHandler.h"

#include <memory>
#include <string>

namespace openpal
{


/**
* A copyable facade over a LogRoot class
*/
class Logger
{
	
public:

	struct Settings
	{
		Settings(const std::string& id, openpal::LogFilters levels) : id(id), levels(levels)
		{}

		std::string id;
		openpal::LogFilters levels;
	};

	Logger(const std::shared_ptr<ILogHandler>& backend, const std::string& id, openpal::LogFilters levels);

	void Log(const LogFilters& filters, char const* location, char const* message, int errorCode = -1);

	Logger Detach(const std::string& id) const
	{
		return Logger(this->backend, std::make_shared<Settings>(id, this->settings->levels));
	}

	bool IsEnabled(const LogFilters& filters) const;

	LogFilters GetFilters() const 
	{
		return this->settings->levels;
	}

	void SetFilters(const openpal::LogFilters& filters)
	{
		this->settings->levels = filters;
	}

	void Rename(const std::string& id)
	{
		this->settings->id = id;
	}

private:

	Logger(const std::shared_ptr<ILogHandler>& backend, const std::shared_ptr<Settings>& settings) :
		backend(backend),
		settings(settings)
	{}

	Logger() = delete;
	Logger& operator=(const Logger&) = delete;

	std::shared_ptr<ILogHandler> backend;
	std::shared_ptr<Settings> settings;
};

}

#endif


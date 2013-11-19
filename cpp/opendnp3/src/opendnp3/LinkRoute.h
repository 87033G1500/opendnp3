
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __LINK_ROUTE_H_
#define __LINK_ROUTE_H_

#include <openpal/Types.h>
#include <openpal/Visibility.h>

#include <iostream>

namespace opendnp3
{

/**
 * Immutable class that defines a route from a DNP3 source address to a
 * destination address.  Remote/Local used here instead of source/destination.
 *
 * When transmitting, destination = remote, source = local
 * When receiving, destination = local, soource = remote
 *
 * Primary used as a key for stl map/set.
 */
class DLL_LOCAL LinkRoute
{
public:
	LinkRoute(const uint16_t aRemoteAddr, const uint16_t aLocalAddr);

	LinkRoute();

	uint16_t remote;
	uint16_t local;

	// comparison functor for use with stl map/set
	struct LessThan {
		// Const to fix VS compilation bug
		bool operator()(const LinkRoute& a, const LinkRoute& b) const;
	};

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::ostream& operator<<(std::ostream& oss, const LinkRoute&);
#endif

}

#endif


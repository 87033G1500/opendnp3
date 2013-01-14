
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//

#ifndef _TIMEOUT_H__
#define _TIMEOUT_H__

#include <chrono>

namespace apl
{

// Use this class to simplify writing do loops with a timeout
// it minimizes the number of calls to get datetime and allows
// us to easily replace the implementation later if we find an
// even more effecient way to implement the timeout checking.
//
// Intended Usage:
//
// Timeout to(5000);
// do{
//	  //call some subordinate slow function
//	  bool success = WaitForInput(to.Remaining());
//
//		//do something on success
//		if(success) return or break;
//
//		//or go back around the loop, the next call to
//		//remaining will be guarnteed to be > 0
// }while(!to.IsExpired());
class Timeout
{
public:
	// constuctor, timeout will expire this many mills in the future
	Timeout(std::chrono::high_resolution_clock::duration aTimeout);

	// returns whether its expired
	bool IsExpired();

	// returns how much time is left
	std::chrono::high_resolution_clock::duration Remaining();
	

private:		

	std::chrono::high_resolution_clock::time_point mExpireTime;

};


}

#endif

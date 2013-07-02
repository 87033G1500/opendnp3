
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

namespace DNP3.Interface
{
	public enum StaticBinaryResponse {
		GROUP1_VAR2 = 0
	};

	public enum StaticAnalogResponse {
		GROUP30_VAR1 = 0,
		GROUP30_VAR2 = 1,
		GROUP30_VAR3 = 2,
		GROUP30_VAR4 = 3,
		GROUP30_VAR5 = 4,
		GROUP30_VAR6 = 5		
	};

	public enum StaticCounterResponse {
		GROUP20_VAR1 = 0,
		GROUP20_VAR2 = 1,
		GROUP20_VAR5 = 2,
		GROUP20_VAR6 = 3
	};

	public enum StaticSetpointStatusResponse {
		GROUP40_VAR1 = 0,
		GROUP40_VAR2 = 1,
		GROUP40_VAR3 = 2,
		GROUP40_VAR4 = 3		
	};

	public enum EventBinaryResponse {
		GROUP2_VAR1 = 0,
		GROUP2_VAR2 = 1
	};

	public enum EventAnalogResponse {
		GROUP32_VAR1 = 0,
		GROUP32_VAR2 = 1,
		GROUP32_VAR3 = 2,
		GROUP32_VAR4 = 3,
		GROUP32_VAR5 = 4,
		GROUP32_VAR6 = 5,
		GROUP32_VAR7 = 6,
		GROUP32_VAR8 = 7
	};

	public enum EventCounterResponse {
		GROUP22_VAR1 = 0,
		GROUP22_VAR2 = 1,
		GROUP22_VAR5 = 2,
		GROUP22_VAR6 = 3
	};	
}




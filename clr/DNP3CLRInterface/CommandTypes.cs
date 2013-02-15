﻿
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{   

    /// <summary>
    /// An enumeration of result codes received from an outstation in response to command request.
    /// These correspond directly to those defined in the dnp3 standard
    /// </summary>
    public enum CommandStatus {
        /// <summary>
        /// command was successfully received and handled
        /// </summary>
	    CS_SUCCESS,			
        /// <summary>
        /// command timeout before completing
        /// </summary>
	    CS_TIMEOUT,	
		/// <summary>
        /// command requires being selected before operate, configuration issue
		/// </summary>
	    CS_NO_SELECT,
		/// <summary>
        /// bad control code or timing values
		/// </summary>
	    CS_FORMAT_ERROR,
	    /// <summary>
        /// command is not supported or implemented
	    /// </summary>
	    CS_NOT_SUPPORTED,
	    /// <summary>
        /// command is already in progress or its already in that mode
	    /// </summary>
	    CS_ALREADY_ACTIVE,
	    /// <summary>
        /// something is stopping the command, often a local/remote interlock
	    /// </summary>
	    CS_HARDWARE_ERROR,
	    /// <summary>
        /// the function governed by the control is in local only control
	    /// </summary>
	    CS_LOCAL,
		/// <summary>
        /// the command has been done too often and has been throttled
		/// </summary>	
	    CS_TOO_MANY_OPS,
	    /// <summary>
        /// the command was rejected because the device denied it or an RTU intercepted it
	    /// </summary>
	    CS_NOT_AUTHORIZED,
        /// <summary>
        /// 10 to 126 are currently reserved
        /// </summary>
	    CS_UNDEFINED
    }

    /// <summary>
    /// Possible action types from a ControlRelayOutputBlock
    /// </summary>
    public enum ControlCode {
        /// <summary>
        /// Only used internally
        /// </summary>
	    CC_NULL,
        /// <summary>
        /// 'push-button' interface, can only be pressed one way (reset button on pedometer)
        /// </summary>
	    CC_PULSE,
        /// <summary>
        /// 'light-switch' moved to the ON position
        /// </summary>
	    CC_LATCH_ON,
        /// <summary>
        /// 'light-switch' moved to the OFF position
        /// </summary>
	    CC_LATCH_OFF,
        /// <summary>
        /// 'doorbell' that rings while the button is depressed
        /// </summary>
	    CC_PULSE_CLOSE,
        /// <summary>
        /// 'doorbell' that stops ringing (is normally on) while depressed
        /// </summary>
	    CC_PULSE_TRIP,
        /// <summary>
        /// 'doorbell' that stops ringing (is normally on) while depressed
        /// </summary>
	    CC_UNDEFINED
    }

    /// <summary>
    /// Command request to control a relay output.
    /// 
    /// Has lots of possible modes. Consult the dnp3 standard for all valid combinations.
    /// </summary>
    public class ControlRelayOutputBlock
    {
        /// <summary>
        /// Primary constructor
        /// </summary>
        /// <param name="code">Enumeration that controls that behavior of the command</param>
        /// <param name="count">How many times to repeat the operation</param>
        /// <param name="onTime"How long to set the output active></param>
        /// <param name="offTime"How long to set the output inactive></param>
        public ControlRelayOutputBlock(ControlCode code, byte count, System.UInt16 onTime, System.UInt16 offTime)
        {
            this.code = code;
            this.count = count;
            this.onTime = onTime;
            this.offTime = offTime;
        }

        public readonly ControlCode code;
        public readonly byte count;
        public readonly System.UInt16 onTime;
        public readonly System.UInt16 offTime;
    }

    /// <summary>
    /// Base class for all AnalogOutputs
    /// </summary>
    /// <typeparam name="T">Type of value sent to the outstation</typeparam>
    public class AnalogOutput<T>
    {	    	
	    public AnalogOutput(T value)
        {
            this.value = value;            
        }	   	    

	    public readonly T value;        
    }
   
    /// <summary>
    /// A set-point command request with short integer representation.
    /// The underlying DNP3 serialization is Group41, Variation 1
    /// </summary>
    public class AnalogOutputInt32 : AnalogOutput<System.Int32>
    {
        public AnalogOutputInt32(System.Int32 value) : base(value)
        {}
    }

    /// <summary>
    /// A set-point command request with short integer representation.
    /// The underlying DNP3 serialization is Group41, Variation 2
    /// </summary>
    public class AnalogOutputInt16 : AnalogOutput<System.Int16>
    {
        public AnalogOutputInt16(System.Int16 value)
            : base(value)
        { }
    }

    /// <summary>
    /// A set-point command request with single precision floating point representation.
    /// The underlying DNP3 serialization is Group41, Variation 3
    /// </summary>
    public class AnalogOutputFloat32 : AnalogOutput<float>
    {
        public AnalogOutputFloat32(float value)
            : base(value)
        { }
    }

    /// <summary>
    /// A set-point command request with double precision floating point representation.
    /// The underlying DNP3 serialization is Group41, Variation 4
    /// </summary>
    public class AnalogOutputDouble64 : AnalogOutput<double>
    {
        public AnalogOutputDouble64(System.Double value)
            : base(value)
        { }
    }

}

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
    /// Interface for dispatching command requests from a master to an outstation
    /// </summary>
    public interface ICommandProcessor
    {
        /// <summary>
        /// Select and operate a ControlRelayOutputBlock
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> SelectAndOperate(ControlRelayOutputBlock command, System.UInt32 index);

        /// <summary>
        /// Select and operate a 32-bit AnalogOutput (Group 41 Var1)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> SelectAndOperate(AnalogOutputInt32 command, System.UInt32 index);

        /// <summary>
        /// Select and operate a 16-bit AnalogOutput (Group 41 Var2)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> SelectAndOperate(AnalogOutputInt16 command, System.UInt32 index);

        /// <summary>
        /// Select and operate a single precision AnalogOutput (Group 41 Var3)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> SelectAndOperate(AnalogOutputFloat32 command, System.UInt32 index);

        /// <summary>
        /// Select and operate a double precision AnalogOutput (Group 41 Var4)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> SelectAndOperate(AnalogOutputDouble64 command, System.UInt32 index);

        /// <summary>
        /// Direct operate a ControlRelayOutputBlock
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> DirectOperate(ControlRelayOutputBlock command, System.UInt32 index);

        /// <summary>
        /// Direct operate a 32-bit AnalogOutput (Group 41 Var1)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> DirectOperate(AnalogOutputInt32 command, System.UInt32 index);

        /// <summary>
        /// Direct operate a 16-bit AnalogOutput (Group 41 Var2)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> DirectOperate(AnalogOutputInt16 command, System.UInt32 index);

        /// <summary>
        /// Direct operate a single precision AnalogOutput (Group 41 Var3)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> DirectOperate(AnalogOutputFloat32 command, System.UInt32 index);

        /// <summary>
        /// Direct operate a double precision AnalogOutput (Group 41 Var4)
        /// </summary>
        /// <param name="command">command to operate</param>
        /// <param name="index">index of command</param>
        /// <returns>A reference to future value of the result</returns>
        IFuture<CommandStatus> DirectOperate(AnalogOutputDouble64 command, System.UInt32 index);
    }
}

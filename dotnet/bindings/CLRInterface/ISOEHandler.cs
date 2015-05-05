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
// you under the terms of the License.
//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// A measurement value and index pair
    /// </summary>
    /// <typeparam name="T">Type of the measurement</typeparam>
    public class IndexedValue<T>
    {
        public IndexedValue(T value, System.UInt16 index)
        {
            this.value = value;
            this.index = index;
        }

        private readonly T value;
        private readonly System.UInt16 index;

        public T Value
        {
            get { return value;  }
        }

        public UInt16 Index
        {
            get { return index; }
        }
    }

    public class HeaderInfo
    {
        public HeaderInfo(GroupVariation variation, QualifierCode qualifier, TimestampMode tsmode, UInt32 headerCount)
        {
            this.variation = variation;
            this.qualifier = qualifier;
            this.tsmode = tsmode;
            this.headerCount = headerCount;
        }
        
        public readonly GroupVariation variation;
        public readonly QualifierCode qualifier;
        public readonly TimestampMode tsmode;
        public readonly UInt32 headerCount;
    }

    /// <summary>
    /// Interface called to receive measurement callbacks from the master
    /// </summary>
    public interface ISOEHandler
	{
            void Start();
            void End();

            void OnValue(HeaderInfo info, Binary value, UInt16 index);
            void OnValue(HeaderInfo info, DoubleBitBinary value, UInt16 index);
            void OnValue(HeaderInfo info, Analog value, UInt16 index);
            void OnValue(HeaderInfo info, Counter value, UInt16 index);
            void OnValue(HeaderInfo info, FrozenCounter value, UInt16 index);
            void OnValue(HeaderInfo info, BinaryOutputStatus value, UInt16 index);
            void OnValue(HeaderInfo info, AnalogOutputStatus value, UInt16 index);
            void OnValue(HeaderInfo info, OctetString value, UInt16 index);
            void OnValue(HeaderInfo info, TimeAndInterval value, UInt16 index);
            void OnValue(HeaderInfo info, BinaryCommandEvent value, UInt16 index);
            void OnValue(HeaderInfo info, AnalogCommandEvent value, UInt16 index);	 
	}
}

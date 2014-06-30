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

namespace DNP3.Interface
{
    /// <summary>
    /// Configuration class for the link layer
    /// </summary>
    public class LinkConfig
	{
        public static System.UInt16 DefaultOutstationAddress
        {
            get {
                return 1024;
            }
        }
        public static System.UInt16 DefaultMasterAddress
        {
            get {
                return 1;
            }
        }

        public static System.UInt16 GetDefaultSourceAddress(bool isMaster)
        {
            return isMaster ? DefaultMasterAddress : DefaultOutstationAddress;
        }

        public static System.UInt16 GetDefaultDestinationAddress(bool isMaster)
        {
            return isMaster ? DefaultOutstationAddress : DefaultMasterAddress;
        }

        public static System.UInt16 GetDefaultAddress(bool isMaster)
        {
            return isMaster ? DefaultMasterAddress : DefaultOutstationAddress;
        }

		/// <summary>
		/// Full constructor
		/// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
        /// <param name="numRetry">The number of retry attempts the link will attempt after the initial try</param>
        /// <param name="localAddr">dnp3 address of the local device</param>
        /// <param name="remoteAddr">dnp3 address of the remote device</param>
        /// <param name="timeout">the response timeout in milliseconds for confirmed requests</param>
		public LinkConfig(	bool isMaster, 
						    bool useConfirms, 
						    System.UInt32 numRetry, 
						    System.UInt16 localAddr, 
						    System.UInt16 remoteAddr, 
						    System.UInt32 timeoutMs)							
		{
            this.isMaster = isMaster;
            this.useConfirms = useConfirms;
            this.numRetry = numRetry;
            this.localAddr = localAddr;
            this.remoteAddr = remoteAddr;
            this.timeoutMs = timeoutMs;
        }

        /// <summary>
        /// Defaults constructor
        /// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
        public LinkConfig(bool isMaster, bool useConfirms)
            : this(isMaster, useConfirms, 0, isMaster ? DefaultMasterAddress : DefaultOutstationAddress, isMaster ? DefaultOutstationAddress : DefaultMasterAddress, 1000)
        {
			
		}

        /// <summary>
        /// Defaults to master
        /// </summary>
        public LinkConfig() : this(true, false)
        { 
        
        }

		/// <summary>
		/// The master/outstation bit set on all messages
		/// </summary>
		public bool isMaster;

		/// <summary>
		/// If true, the link layer will send data requesting confirmation
		/// </summary> 
		public bool useConfirms;

		/// <summary>
		/// The number of retry attempts the link will attempt after the initial try
		/// </summary>
		public System.UInt32 numRetry;

		/// <summary>
		/// dnp3 address of the local device
		/// </summary>
		public System.UInt16 localAddr;

		/// <summary>
		/// dnp3 address of the remote device
		/// </summary>
		public System.UInt16 remoteAddr;

		/// <summary>
		/// the response timeout for confirmed requests
		/// </summary>
		public System.UInt32 timeoutMs;
	}

    /// <summary>
    /// Configuration object for the application layer
    /// </summary>
	public class AppConfig
	{
        /// <summary>
        /// Constructor that defaults to master
        /// </summary>
        public AppConfig()
            : this(true, 5000, 0, 2048)
        {
        }

	    /// <summary>
	    /// Constructor with reasonable defaults
	    /// </summary>
        public AppConfig(bool isMaster) : this(isMaster, 5000, 0, 2048)
        {           
        }

        /// <summary>
        /// Constructor with all values parameterized
        /// </summary>
        /// <param name="rspTimeout"> The response/confirm timeout in millisec</param>
        /// <param name="numRetry">Number of retries performed for applicable frames</param>
        /// <param name="fragSize">The maximum size of received application layer fragments</param>
		public AppConfig(bool isMaster, System.UInt32 rspTimeoutMs, System.Int32 numRetry, System.Int32 fragSize)
        {
            this.isMaster = isMaster;
			this.rspTimeoutMs = rspTimeoutMs;
			this.numRetry = numRetry;
			this.fragSize = fragSize;
        }
        
        /// <summary>
        /// Configures the app layer for master or outstation operation
        /// </summary>
        public bool isMaster;

		/// <summary>
		/// The response/confirm timeout in millisec
		/// </summary>
        public System.UInt32 rspTimeoutMs;

		/// <summary>
		/// Number of retries performed for applicable frames
		/// </summary>
        public System.Int32 numRetry;

		/// <summary>
		/// The maximum size of received application layer fragments
		/// </summary>
        public System.Int32 fragSize;
	}           

    /// <summary>
    /// Class that defines how many events an outstation will before losing events
    /// </summary>
    public class EventBufferConfig {

        /// <summary>
        /// All events set to 1000
        /// </summary>
        public EventBufferConfig()
        {
            this.maxBinaryEvents = 1000;
            this.maxDoubleBinaryEvents = 1000;
            this.maxAnalogEvents = 1000;
            this.maxCounterEvents = 1000;
            this.maxFrozenCounterEvents = 1000;
            this.maxBinaryOutputStatusEvents = 1000;
            this.maxAnalogOutputStatusEvents = 1000;
        }

	    /// <summary>
	    /// The number of binary events the outstation will buffer before overflowing
	    /// </summary>
	    public System.UInt32 maxBinaryEvents;

        /// <summary>
        /// The number of double-bit binary events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxDoubleBinaryEvents;

	    /// <summary>
	    /// The number of analog events the outstation will buffer before overflowing
	    /// </summary>
        public System.UInt32 maxAnalogEvents;

	    /// <summary>
	    /// The number of counter events the outstation will buffer before overflowing
	    /// </summary>
        public System.UInt32 maxCounterEvents;

        /// <summary>
        /// The number of frozen counter events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxFrozenCounterEvents;

        /// <summary>
        /// The number of binary output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxBinaryOutputStatusEvents;

        /// <summary>
        /// The number of analog output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxAnalogOutputStatusEvents;
    }


    /// <summary>
    /// Configuration information for a dnp3 outstation (outstation)
    /// Used as both input describing the startup configuration of the outstation, and as configuration state of mutable properties (i.e. unsolicited responses).
    /// Major feature areas are unsolicited responses, time synchronization requests, event buffer limits, and the DNP3 object/variations to use by default
    /// when the master requests class data or variation 0.
    /// </summary>
    public class OutstationConfig 
    {
        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public OutstationConfig()
        {
            this.maxControls = 1;
            this.disableUnsol = false;
            this.unsolMask = new ClassField(0);
            this.allowTimeSync = false;
            this.timeSyncPeriodMs = 10*60000;
            this.unsolPackDelayMs = 200;
            this.unsolRetryDelayMs = 2000;
            this.selectTimeoutMs = 5000;
            this.maxFragSize = 2048;            
            this.staticBinary = StaticBinaryResponse.Group1Var2;
            this.staticAnalog = StaticAnalogResponse.Group30Var1;
            this.staticCounter = StaticCounterResponse.Group20Var1;
            this.staticAnalogOutputStatus = StaticAnalogOutputStatusResponse.Group40Var1;
            this.eventBinary = EventBinaryResponse.Group2Var1;
            this.eventAnalog = EventAnalogResponse.Group32Var1;
            this.eventCounter = EventCounterResponse.Group22Var1;
        }

	    /// <summary>
        /// The maximum number of controls the outstation will attempt to process from a single APDU
	    /// </summary>
	    public System.UInt32 maxControls;

	    /// <summary>
        /// if true, fully disables unsolicited mode as if the outstation didn't support it
	    /// </summary>
        public bool disableUnsol;

	    /// <summary>
        /// controls what unsol classes are enabled
	    /// </summary>
        public ClassField unsolMask;

	    /// <summary>
        /// if true, the outstation will request time synchronization on an interval
	    /// </summary>
        public bool allowTimeSync;

	    /// <summary>
        /// The period of time sync interval in milliseconds
	    /// </summary>
        public System.UInt32 timeSyncPeriodMs;

	    /// <summary>
        /// The amount of time the outstation will wait before sending new unsolicited data (less than 0 == immediate)
	    /// </summary>
        public System.UInt32 unsolPackDelayMs;

	    /// <summary>
        /// How long the outstation will wait before retrying an unsuccessful unsol response
	    /// </summary>
        public System.UInt32 unsolRetryDelayMs;

        /// <summary>
        /// How long the outstation will allow an operate to proceed after a prior select
        /// </summary>
        public System.UInt32 selectTimeoutMs;

	    /// <summary>
        /// The maximum fragment size the outstation will use for data it sends
	    /// </summary>
        public System.UInt32 maxFragSize;	    	    

	    /// <summary>
        /// The default group/variation to use for static binary responses
	    /// </summary>	    
	    public StaticBinaryResponse staticBinary;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
	    public StaticAnalogResponse staticAnalog;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
        public StaticCounterResponse staticCounter;

        /// <summary>
        /// The default group/variation to use for static analog responses
        /// </summary>
        public StaticFrozenCounterResponse staticFrozenCounter;

	    /// <summary>
        /// The default group/variation to use for static setpoint status responses
	    /// </summary>
        public StaticAnalogOutputStatusResponse staticAnalogOutputStatus;

	    /// <summary>
        /// The default group/variation to use for binary event responses
	    /// </summary>
        public EventBinaryResponse eventBinary;

	    /// <summary>
        /// The default group/variation to use for analog event responses
	    /// </summary>
        public EventAnalogResponse eventAnalog;

	    /// <summary>
        /// The default group/variation to use for counter event responses
	    /// </summary>
	    public EventCounterResponse eventCounter;	   	    
    }  

    /// <summary>
    /// Point record type that is assigned an event class 
    /// </summary>
    public class EventPointRecord
    {
        public EventPointRecord(PointClass pointClass)
        {            
            this.pointClass = pointClass;
        }

        public EventPointRecord()
            : this(PointClass.Class0)
        { 
        
        }

        public PointClass pointClass;
    };

    /// <summary>
    /// Point record type that is assigned an event class and deadband tolerance
    /// </summary>
    public class DeadbandEventPointRecord<T> : EventPointRecord
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="pointClass">Class that will be evented when the value changes</param>
        /// <param name="deadband">Value can change within this tolerance without producing an event</param>
        public DeadbandEventPointRecord(PointClass pointClass, T deadband) : base(pointClass)
        {
            this.deadband = deadband;
        }

        /// <summary>
        /// Default constructor with Class 0 and 0.1 tolerance
        /// </summary>
        public DeadbandEventPointRecord(): base(PointClass.Class0)
        {
            this.deadband = default(T);
        }

        /// <summary>
        /// Value can change within this tolerance without producing an event
        /// </summary>
        public T deadband;
    };    

    /// <summary>
    /// Defines the database layout for an outstation
    /// </summary>
    public class DatabaseTemplate
    {
        /// <summary>
        /// Constructor that sets up the size of the types
        /// </summary>
        /// <param name="numBinary">numer of binary values starting at index 0</param>
        /// <param name="numAnalog">numer of analog values starting at index 0</param>
        /// <param name="numCounter">numer of counter values starting at index 0</param>
        /// <param name="numCounter">numer of frozen counter values starting at index 0</param>
        /// <param name="numBinaryOutputStatus">numer of control status values starting at index 0</param>
        /// <param name="numAnalogOutputStatus">numer of setpoint status values starting at index 0</param>
        public DatabaseTemplate(  System.UInt16 numBinary,
                                  System.UInt16 numDoubleBinary,
                                  System.UInt16 numAnalog,
                                  System.UInt16 numCounter,
                                  System.UInt16 numFrozenCounter,
                                  System.UInt16 numBinaryOutputStatus,
                                  System.UInt16 numAnalogOutputStatus)
        {
            binaries = Enumerable.Range(0, numBinary).Select(i => new EventPointRecord(PointClass.Class1)).ToList();
            doubleBinaries = Enumerable.Range(0, numDoubleBinary).Select(i => new EventPointRecord(PointClass.Class1)).ToList();
            counters = Enumerable.Range(0, numCounter).Select(i => new DeadbandEventPointRecord<System.UInt32>(PointClass.Class1, 0)).ToList();
            frozenCounters = Enumerable.Range(0, numFrozenCounter).Select(i => new DeadbandEventPointRecord<System.UInt32>(PointClass.Class1, 0)).ToList();
            analogs = Enumerable.Range(0, numAnalog).Select(i => new DeadbandEventPointRecord<double>(PointClass.Class1, 0.0)).ToList();
            binaryOutputStatii = Enumerable.Range(0, numBinaryOutputStatus).Select(i => new EventPointRecord(PointClass.Class1)).ToList();
            analogs = Enumerable.Range(0, numAnalogOutputStatus).Select(i => new DeadbandEventPointRecord<double>(PointClass.Class1, 0.0)).ToList();
        }

        /// <summary>
        /// Default constructor that sets up 10 of every type
        /// </summary>
        public DatabaseTemplate()
            : this(10, 10, 10, 10, 10, 10, 10)
        { }

        /// <summary>
        /// Modify individual binary configuration here
        /// </summary>
        public List<EventPointRecord> binaries;
        /// <summary>
        /// Modify individual double binary configuration here
        /// </summary>
        public List<EventPointRecord> doubleBinaries;
        /// <summary>
        /// Modify individual analog configuration here
        /// </summary>
        public List<DeadbandEventPointRecord<System.UInt32>> counters;
        /// <summary>
        /// Modify individual analog configuration here
        /// </summary>
        public List<DeadbandEventPointRecord<System.UInt32>> frozenCounters;
        /// <summary>
        /// Modify individual counter configuration here
        /// </summary>
        public List<DeadbandEventPointRecord<double>> analogs;
        /// <summary>
        /// Modify individual binary output status configuration here
        /// </summary>
        public List<EventPointRecord> binaryOutputStatii;
        /// <summary>
        /// Modify individual analog output status configuration here
        /// </summary>
        public List<DeadbandEventPointRecord<double>> analogOutputStatii;
    };

    /// <summary>
    /// Aggreate configuration for a master stack
    /// </summary>
	public class MasterStackConfig
    {	
        /// <summary>
        /// Reasonable defaults
        /// </summary>
		public MasterStackConfig()
        {
            this.link = new LinkConfig(true, false);
            this.master = new MasterConfig();
            this.app = new AppConfig(true);
		}        

        /// <summary>
        /// Configuration for a master
        /// </summary>
        public MasterConfig master;
        /// <summary>
        /// Configuration for the application layer
        /// </summary>
        public AppConfig app;
        /// <summary>
        /// Configuration for the link layer
        /// </summary>
        public LinkConfig link;
	}

    /// <summary>
    /// Aggreate configuration for an outstation stack
    /// </summary>
    public class OutstationStackConfig
	{
        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public OutstationStackConfig()
        {
            this.outstation = new OutstationConfig();
            this.buffer = new EventBufferConfig();
            this.device = new DatabaseTemplate(10, 10, 10, 10, 10, 10, 10);
            this.link = new LinkConfig(false, false);
            this.app = new AppConfig(false);            
        }

        /// <summary>
        /// Outstation config
        /// </summary>
	    public OutstationConfig outstation;
        /// <summary>
        /// Configuration of the outstation event buffer
        /// </summary>
        public EventBufferConfig buffer;
        /// <summary>
        /// Device template that specifies database layout, control behavior
        /// </summary>
        public DatabaseTemplate device;
        /// <summary>
        /// Application layer config
        /// </summary>
        public AppConfig app;
        /// <summary>
        /// Link layer config
        /// </summary>
        public LinkConfig link;
	}
}

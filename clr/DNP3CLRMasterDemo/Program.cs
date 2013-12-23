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

using DNP3.Adapter;
using DNP3.Interface;

namespace DotNetMasterDemo
{
    class Program
    {
        static int Main(string[] args)
        {
            IDNP3Manager mgr = DNP3ManagerFactory.CreateManager();            
            mgr.AddLogHandler(PrintingLogAdapter.Instance); //this is optional
            var channel = mgr.AddTCPClient("client", LogLevel.Info, TimeSpan.FromSeconds(5), "127.0.0.1", 20000);

            //optionally, add a listener for the channel state
            channel.AddStateListener(state => Console.WriteLine("Client state: " + state));

            var config = new MasterStackConfig();
            config.master.integrityPeriod = TimeSpan.FromSeconds(60);
            config.link.useConfirms = true; //setup your stack configuration here.
            var master = channel.AddMaster("master", LogLevel.Interpret, PrintingMeasurementHandler.Instance, config);

            //optionally, add a listener for the stack state
            master.AddStateListener(state => Console.WriteLine("Master state: " + state));

            var classMask = PointClassHelpers.GetMask(PointClass.PC_CLASS_1, PointClass.PC_CLASS_2, PointClass.PC_CLASS_3);
            var classScan = master.AddClassScan(classMask, TimeSpan.FromSeconds(5), TimeSpan.FromSeconds(5));
            var integrityScan = master.GetIntegrityScan();

            classScan.AddScanCallback((ScanResult result) => Console.WriteLine("class scan result: " + result.Status));

            master.Enable(); // enable communications

            Console.WriteLine("Enter an index to send a command");

            while (true)
            {                
                switch (Console.ReadLine())
                { 
                    case "c":
                        var crob = new ControlRelayOutputBlock(ControlCode.PULSE, 1, 100, 100);
                        var future = master.GetCommandProcessor().SelectAndOperate(crob, 0);
                        future.Listen((result) => Console.WriteLine("Result: " + result));
                        break;
                    case "i":
                        integrityScan.Demand();
                        break;
                    case "e":
                        classScan.Demand();                        
                        break;
                    case "x":
                        return 0;                        
                    default:
                        break;
                }                           
            }            
        }
    }
}

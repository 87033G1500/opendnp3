﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class OutstationNode : ISimulatorNode
    {
        readonly IDNP3Config config;
        readonly IOutstation outstation;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;
        readonly Guid guid = new Guid();

        string ISimulatorNode.Alias
        {
            get
            {
                return alias;
            }
        }

        public OutstationNode(IDNP3Config config, IOutstation outstation, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.config = config;
            this.outstation = outstation;
            this.callbacks = callbacks;
            this.alias = alias;

            this.callbacks.ChangeImage(IconIndex.Outstation);            
        }
        
        void ISimulatorNode.Remove()
        {
            outstation.Shutdown();            
        }

        IEnumerable<Metric> ISimulatorNode.Metrics
        {
            get
            {
                var list = new List<Metric>();
                var stats = outstation.GetStackStatistics();
                list.Add(new Metric("Num transport rx", stats.NumTransportRx.ToString()));
                list.Add(new Metric("Num transport tx", stats.NumTransportTx.ToString()));
                list.Add(new Metric("Num transport error rx", stats.NumTransportErrorRx.ToString()));
                return list;
            }
        }

        string ISimulatorNode.DisplayName
        {
            get { return alias; }
        }

        IEnumerable<ISimulatorNodeAction> ISimulatorNode.Actions
        {
            get
            {
                return Enumerable.Empty<ISimulatorNodeAction>();
            }
        }

        IEnumerable<ISimulatorNodeFactory> ISimulatorNode.Children
        {
            get { return Enumerable.Empty<ISimulatorNodeFactory>(); }
        }


        Guid ISimulatorNode.UniqueID
        {
            get { return guid; }
        }
    }
}

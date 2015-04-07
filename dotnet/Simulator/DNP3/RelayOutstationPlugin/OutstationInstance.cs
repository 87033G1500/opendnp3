﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{
    public class OutstationModule : IOutstationModule
    {
        const string name = "Relay Simulator";

        OutstationStackConfig IOutstationModule.DefaultConfig
        {
            get {
                return new OutstationStackConfig();
            }
        }

        bool IOutstationModule.AllowTemplateEditing
        {
            get { return true; }
        }

        IOutstationFactory IOutstationModule.CreateFactory()
        {
            return new OutstationFactory();
        }

        public static IOutstationModule Instance
        {
            get
            {
                return instance;
            }
        }
       
        private OutstationModule()
        {}

        private static OutstationModule instance = new OutstationModule();

        public override string ToString()
        {
            return name;
        }

        string IOutstationModule.Description
        {
            get { return "A simple relay simulator with a mock front panel. Responds to latch on/off"; }
        }

        string IOutstationModule.Name
        {
            get { return name; }
        }
    }


    class OutstationFactory : IOutstationFactory
    {
        private readonly ProxyCommandHandler commandHandler = new ProxyCommandHandler();
        private readonly EventedOutstationApplication application = new EventedOutstationApplication();


        ICommandHandler IOutstationFactory.CommandHandler
        {
            get { return commandHandler; }
        }

        IOutstationApplication IOutstationFactory.Application
        {
            get { return application; }
        }

        IOutstationInstance IOutstationFactory.CreateInstance(IOutstation outstation, string name, OutstationStackConfig config)
        {
            return new OutstationInstance(commandHandler, application, outstation, config, name);
        }
    }

    
    class OutstationInstance : IOutstationInstance
    {        
        readonly ProxyCommandHandler handler;
        readonly EventedOutstationApplication application;
        readonly IOutstation outstation;
        readonly string alias;        

        OutstationForm form = null;
        
        public OutstationInstance(ProxyCommandHandler handler, EventedOutstationApplication application, IOutstation outstation, OutstationStackConfig config, string alias)
        {
            this.handler = handler;
            this.application = application;
            this.outstation = outstation;
            this.alias = alias;            
        }

        string IOutstationInstance.DisplayName
        {
            get { return alias; }
        }

        bool IOutstationInstance.HasForm
        {
            get { return true; }
        }

        void IOutstationInstance.ShowForm()
        {
            if(this.form == null)
            {
                this.form = new OutstationForm();
            }

            form.Show();
        }

        void IOutstationInstance.Shutdown()
        {            
            if (form != null)
            {
                form.Close();
                form.Dispose();
                form = null;
            }         
        }
    }
    
}

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    public class OutstationModule : IOutstationModule
    {
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
            return new OutstationInstance(commandHandler, application, outstation, name);
        }
    }

    
    class OutstationInstance : IOutstationInstance
    {
        
        readonly MeasurementCache cache = new MeasurementCache();
        readonly ProxyCommandHandler handler;
        readonly EventedOutstationApplication application;
        readonly IOutstation outstation;
        readonly string alias;        

        OutstationForm form = null;
        
        public OutstationInstance(ProxyCommandHandler handler, EventedOutstationApplication application, IOutstation outstation, string alias)
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
                this.form = new OutstationForm(outstation, application, cache, handler, alias);
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

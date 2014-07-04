﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.Simulator.API
{
    public interface ISimulatorPluginFactory
    {
        ISimulatorPlugin Create();        
    }
}

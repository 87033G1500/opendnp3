﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Adapter;
using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class DNP3SimulatorPlugin : ISimulatorPlugin
    {
        readonly ImageList imgList = new ImageList();
        readonly IDNP3Manager manager = DNP3ManagerFactory.CreateManager();

        public DNP3SimulatorPlugin()
        {
            imgList.Images.Add(Properties.Resources.satellite_dish);
            imgList.Images.Add(Properties.Resources.network_monitor);
        }

        string ISimulatorPlugin.UniqueId
        {
            get { return "DNP3"; }
        }

        System.Drawing.Image ISimulatorPlugin.PluginImage
        {
            get { return Properties.Resources.satellite_dish_add; }
        }


        string ISimulatorPlugin.RootDisplayName
        {
            get { return "DNP3 Channel"; }
        }


        ISimulatorNode ISimulatorPlugin.Create(ISimulatorNodeCallbacks callbacks)
        {
            using (var dialog = new Components.ChannelDialog(Enumerable.Empty<String>()))
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var channel = dialog.ChannelAction.Invoke(manager);
                    return new ChannelNode(channel, callbacks, "channel");
                }
                else
                {
                    return null;    
                }
            }            
       }

       System.Windows.Forms.ImageList ISimulatorPlugin.NodeImageList
       {
           get { return imgList; }
       }
    }
}

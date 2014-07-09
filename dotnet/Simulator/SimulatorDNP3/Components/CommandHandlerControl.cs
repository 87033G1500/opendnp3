﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    partial class CommandHandlerControl : UserControl
    {   
        ProxyCommandHandler handler = null;
        IDatabase database = null;

        public CommandHandlerControl()
        {
            InitializeComponent();                        
        }

        public void Configure(ProxyCommandHandler proxy, IDatabase database)
        {
            this.handler = proxy;
            this.database = database;
            this.handler.BinaryCommandAccepted += handler_BinaryCommandAccepted;
            this.handler.AnalogCommandAccepted += handler_AnalogCommandAccepted;
        }

        void handler_AnalogCommandAccepted(double value, ushort index, CommandStatus result)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke(new Action(() => handler_AnalogCommandAccepted(value, index, result)));
            }
            else
            {
                var output = String.Format("Accepted Analog: {0} - {1} with result {2}", value, index, result);
                this.listBoxLog.Items.Add(output);
                if (checkBoxMapAnalog.Checked)
                {
                    database.Start();
                    database.Update(new Analog(value, 0x01, DateTime.Now), index);
                    database.End();
                }
            }
        }

        void handler_BinaryCommandAccepted(ControlRelayOutputBlock crob, ushort index, CommandStatus result)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke(new Action(() => handler_BinaryCommandAccepted(crob, index, result)));
            }
            else
            {
                var output = String.Format("Accepted CROB: {0} - {1} with result {2}", crob.code, index, result);
                this.listBoxLog.Items.Add(output);
                if (checkBoxMapBinary.Checked)
                {
                    switch (crob.code)
                    { 
                        case(ControlCode.LATCH_ON):
                            this.LoadSingleBinaryOutputStatus(true, index);
                            break;
                        case (ControlCode.LATCH_OFF):
                            this.LoadSingleBinaryOutputStatus(false, index);
                            break;                        
                    }
                    
                }
            }
        }

        void LoadSingleBinaryOutputStatus(bool value, ushort index)
        {
            database.Start();
            database.Update(new BinaryOutputStatus(value, 0x01, DateTime.Now), index);
            database.End();
        }

        private void checkBoxEnabled_CheckedChanged(object sender, EventArgs e)
        {
            this.handler.Enabled = this.checkBoxEnabled.Checked;
        }

        private IEnumerable<string> MakeHandlerStrings(string id, IEnumerable<KeyValuePair<ushort, CommandStatus>> pairs)
        {
            return pairs.Select(kvp => String.Format("{0} {1} -> {2}", id, kvp.Key, kvp.Value));
        }

        private void RepopulateList()
        {
            this.listBoxHandlers.SuspendLayout();
            this.listBoxHandlers.Items.Clear();
            this.listBoxHandlers.Items.AddRange(MakeHandlerStrings("BO", this.handler.BinaryResponses).ToArray());
            this.listBoxHandlers.Items.AddRange(MakeHandlerStrings("AO", this.handler.AnalogResponses).ToArray());            
            this.listBoxHandlers.ResumeLayout();
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            this.handler.ClearResponses();
            this.RepopulateList();
        }     

        private void buttonAddBO_Click(object sender, EventArgs e)
        {
            using (var dialog = new CommandHandlerDialog("Add CROB Handler"))
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.handler.AddBinaryResponse(dialog.SelectedIndex, dialog.SelectedCode);
                    this.RepopulateList();
                }
            }
            
        }

        private void buttonAddAO_Click(object sender, EventArgs e)
        {
            using (var dialog = new CommandHandlerDialog("Add AnalogOutput Handler"))
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.handler.AddAnalogResponse(dialog.SelectedIndex, dialog.SelectedCode);
                    this.RepopulateList();
                }
            }            
        }
    }
}

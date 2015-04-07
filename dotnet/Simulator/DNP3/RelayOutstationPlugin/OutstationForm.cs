﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{

    public partial class OutstationForm : Form, ICommandHandler
    {
        readonly IOutstation outstation;
        readonly ProxyCommandHandler commandHandler;
        
        static readonly double[] currentValues = new double[] { 1243, 1237, 1241, 1242 };

        static readonly Rectangle tripBounds = new Rectangle(467, 174, 32, 32);
        static readonly Rectangle closeBounds = new Rectangle(467, 133, 32, 32);

        
        IEnumerator<double> currentA = InfiniteEnumerable.Create(currentValues).GetEnumerator();
        IEnumerator<double> currentB = InfiniteEnumerable.Create(currentValues).Skip(1).GetEnumerator();
        IEnumerator<double> currentC = InfiniteEnumerable.Create(currentValues).Skip(2).GetEnumerator();

        bool closed = true;

        public OutstationForm(IOutstation outstation, ProxyCommandHandler commandHandler)
        {
            InitializeComponent();

            this.outstation = outstation;
            this.commandHandler = commandHandler;

            commandHandler.CommandProxy = this;

            this.UpdateValues();
        }

        private void transparentButtonTrip_Click(object sender, EventArgs e)
        {
            this.closed = false;
            this.UpdateValues();
        }

        private void transparentButtonClose_Click(object sender, EventArgs e)
        {
            this.closed = true;
            this.UpdateValues();
        }

        private void UpdateValues()
        {
            var changes = new ChangeSet();
            var now = DateTime.Now;            

            if (closed)
            {
                changes.Update(new Analog(currentA.Current, 1, now), 0);
                changes.Update(new Analog(currentB.Current, 1, now), 1);
                changes.Update(new Analog(currentC.Current, 1, now), 2);

                this.labelValueIA.Text = String.Format("IA: {0}", currentA.Current);
                this.labelValueIB.Text = String.Format("IB: {0}", currentB.Current);
                this.labelValueIC.Text = String.Format("IC: {0}", currentC.Current);

                currentA.MoveNext();
                currentB.MoveNext();
                currentC.MoveNext();
            }
            else
            {
                changes.Update(new Analog(0, 1, now), 0);
                changes.Update(new Analog(0, 1, now), 1);
                changes.Update(new Analog(0, 1, now), 2);

                this.labelValueIA.Text = String.Format("IA: {0}", 0);
                this.labelValueIB.Text = String.Format("IB: {0}", 0);
                this.labelValueIC.Text = String.Format("IC: {0}", 0);
            }

            outstation.Load(changes);
        }

        private void OutstationForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }

        CommandStatus ICommandHandler.Select(ControlRelayOutputBlock command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt16 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputFloat32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputDouble64 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(ControlRelayOutputBlock command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt16 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputFloat32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputDouble64 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        private void timerUpdate_Tick(object sender, EventArgs e)
        {
            this.UpdateValues();
        }

        private void pictureBoxRelay_MouseMove(object sender, MouseEventArgs e)
        {
            if (tripBounds.Contains(e.Location))
            {
                this.Cursor = Cursors.Hand;
            }
            else if (closeBounds.Contains(e.Location))
            {
                this.Cursor = Cursors.Hand;
            }
            else
            {
                this.Cursor = Cursors.Default;
            }
        }       

        private void pictureBoxRelay_MouseClick(object sender, MouseEventArgs e)
        {
            if (tripBounds.Contains(e.Location))
            {
                this.closed = false;
                this.UpdateValues();
            }
            else if (closeBounds.Contains(e.Location))
            {
                this.closed = true;
                this.UpdateValues();
            }
        }
       
    }
}

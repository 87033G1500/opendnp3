﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Automatak.Simulator.UI
{
    public partial class LogWindow : UserControl
    {
        public LogWindow()
        {
            InitializeComponent();
            this.UpdateStatus();
        }

        public void AddRows(IEnumerable<string> lines)
        {
            this.logControl.AddRows(lines);
            this.UpdateStatus();
        }        

        public Font LogFont
        {
            get { return logControl.Font; }
            set
            {
                logControl.Font = value;
            }
        }

        void UpdateStatus()
        { 
            var format = "{0,5} / {1,-5}";

            this.toolStripStatusLabel.Text = String.Format(
                format, 
                logControl.NumRows,
                logControl.MaxRows);            
        }

        private void buttonPause_Click(object sender, EventArgs e)
        {
            this.Pause();  
        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            this.Resume();
        }

        void Pause()
        {
            this.buttonPause.Enabled = false;            
            this.buttonPlay.Enabled = true;
            this.pauseToolStripMenuItem.Enabled = false;
            this.resumeToolStripMenuItem.Enabled = true;
            this.logControl.Pause();    
        }

        void Resume()
        {
            this.buttonPause.Enabled = true;
            this.buttonPlay.Enabled = false;
            this.pauseToolStripMenuItem.Enabled = true;
            this.resumeToolStripMenuItem.Enabled = false;
            this.logControl.Resume();
        }

        private void buttonClipboard_Click(object sender, EventArgs e)
        {
            this.CopyToClipboard();
        }

        void CopyToClipboard()
        {
            var rows = this.logControl.GetAllRows();
            Clipboard.SetText(String.Join(Environment.NewLine, rows.ToArray()));
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            logControl.Clear();
        }

        private void vScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            if (e.NewValue != e.OldValue)
            {
                int userMax = (vScrollBar.Maximum + 1) - vScrollBar.LargeChange;

                double fraction = ((double) e.NewValue) / ((double) userMax);

                logControl.SetViewport(fraction);                
            }            
        }

        private void pauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Pause();
        }

        private void resumeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Resume();
        }

        private void pauseToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            this.Pause();
        }

        private void resumeToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            this.Resume();
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            logControl.Clear();
        }

        private void copyToClipboardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.CopyToClipboard();
        }
    }
}

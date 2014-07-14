﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public partial class DoubleBinaryValueDialog : Form
    {        
        readonly IEnumerable<ushort> indices;

        public DoubleBinaryValueDialog(IEnumerable<ushort> indices)
        {
            InitializeComponent();
            
            this.indices = indices;

            this.qualitySelector.QualityInfo = QualityInfo.doubleBinary;

            this.comboBox1.DataSource = Enum.GetValues(typeof(DoubleBit));
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public IEnumerable<Action<IDatabase>> SelectedActions
        {
            get
            {
                var value = (DoubleBit) comboBox1.SelectedItem;
                var quality = qualitySelector.Quality;
                var timestamp = DateTime.Now;
                
                return indices.Select(i => MeasActions.GetDoubleBinaryAction(value, quality, timestamp, i));
            }
        }        
    }
}

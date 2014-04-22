﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;

namespace DNP3TestHarness
{
    partial class About : Form
    {
        public About()
        {
            InitializeComponent();

            string[] lines = {
                 "This software licensed to you under the Apache 2.0 license:",
                 "",
                 "http://www.apache.org/licenses/LICENSE-2.0",
                 "",
                 "This software contains softwre written by:",
                 "",
                 "Automatak LLC - http://www.automatak.com",
                 "Green Enery Corp - http://www.greenenergycorp.com",
                 "asio C++ library - http://think-async.com/",
                 "",
                 "Unless required by applicable law or agreed to in writing, software",
                 "distributed under the License is distributed on an \"AS IS\" BASIS",
                 "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
            };


            this.richTextBox1.Text = String.Join(Environment.NewLine, lines);
        }        
    }
}

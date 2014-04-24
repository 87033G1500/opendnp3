﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    class MeasurementPoco : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(string name)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        public MeasurementPoco(int row, string sValue, MeasurementBase meas, UInt16 index)
        {
            this.sValue = sValue;
            this.index = index;
            this.flags = meas.Quality.ToString("X2");
            this.timeStamp = meas.Timestamp;
            this.row = row;
            this.isValid = true;
        }

        public MeasurementPoco()
        {
            this.isValid = false;
        }

        public void Update(string sValue, MeasurementBase meas)
        {
            Value = sValue;
            Flags = meas.Quality.ToString("X2");
            TimeStamp = meas.Timestamp;
        }

        private UInt16 index;
        private string sValue;
        private string flags;
        private DateTime timeStamp;
        private int row;
        private bool isValid;

        public void NextRow()
        {
            ++row;
        }

        public Boolean IsValid
        {
            get { return isValid; }
        }

        public UInt16 Index
        {
            get { return index; }
            set { 
                index = value;
                NotifyPropertyChanged("Index");
            }
        }

        public string Value
        {
            get { return sValue; }
            set { 
                sValue = value;
                NotifyPropertyChanged("Value");
            }
        }

        public string Flags
        {
            get { return flags; }
            set { 
                flags = value;
                NotifyPropertyChanged("Flags");
            }
        }

        public DateTime TimeStamp
        {
            get { return timeStamp; }
            set { 
                timeStamp = value;
                NotifyPropertyChanged("TimeStamp");
            }
        }

        public int Row
        {
            get { return row; }
            set { row = value; }
        }       
    }
}

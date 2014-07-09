﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public enum CommandType
    { 
        CROB,
        AnalogInt16,
        AnalogInt32,
        AnalogSingle,
        AnalogDouble
    }

    class ProxyCommandHandler : ICommandHandler
    {
        bool enabled = false;
        Object mutex = new Object();        
        IDictionary<UInt16, CommandStatus> binaryMap = new Dictionary<UInt16, CommandStatus>();
        IDictionary<UInt16, CommandStatus> analogMap = new Dictionary<UInt16, CommandStatus>();
        ICommandHandler proxy = RejectingCommandHandler.Instance;

        public delegate void OnBinaryAccepted(ControlRelayOutputBlock crob, UInt16 index);
        public event OnBinaryAccepted BinaryCommandAccepted;

        public delegate void OnAnalogAccepted(double value, UInt16 index);
        public event OnAnalogAccepted AnalogCommandAccepted;

        public ICommandHandler CommandProxy
        {
            set
            {
                lock (mutex)
                {
                    if (value == null)
                    {
                        proxy = RejectingCommandHandler.Instance;
                    }
                    else
                    {
                        proxy = value;
                    }
                }
            }
        }       

        public bool Enabled
        {
            get
            {
                return enabled;
            }
            set
            {
                enabled = true;
            }
        }

        public void AddBinaryResponse(UInt16 index, CommandStatus status)
        {
            lock (mutex)
            {
                binaryMap[index] = status;
            }
        }

        public void AddAnalogResponse(UInt16 index, CommandStatus status)
        {
            lock (mutex)
            {
                analogMap[index] = status;
            }
        }

        public void ClearResponses()
        {
            lock (mutex)
            {
                binaryMap.Clear();
                analogMap.Clear();
            }
        }

        public KeyValuePair<UInt16, CommandStatus>[] BinaryResponses
        {
            get
            {
                lock (mutex)
                {
                    return binaryMap.Select(x => x).ToArray();
                }
            }
        }

        public KeyValuePair<UInt16, CommandStatus>[] AnalogResponses
        {
            get
            {
                lock (mutex)
                {
                    return analogMap.Select(x => x).ToArray();
                }
            }
        }
       
        CommandStatus GetOrElse(ushort index, IDictionary<UInt16, CommandStatus> dictionary, Func<CommandStatus> action)
        {
            if (enabled)
            {
                CommandStatus status;
                if (dictionary.TryGetValue(index, out status))
                {                    
                    return status;
                }
                else
                {
                    return action.Invoke();
                }
            }
            else
            {
                return action.Invoke();
            } 
        }

        CommandStatus GetOrElseAndLogBinary(ControlRelayOutputBlock command, ushort index, IDictionary<UInt16, CommandStatus> dictionary, Func<CommandStatus> action)
        {
            var result = GetOrElse(index, dictionary, action);
            if (result == CommandStatus.SUCCESS && BinaryCommandAccepted != null)
            {
                BinaryCommandAccepted(command, index);
            }
            return result;
        }

        CommandStatus GetOrElseAndLogAnalog(double value, ushort index, IDictionary<UInt16, CommandStatus> dictionary, Func<CommandStatus> action)
        {
            var result = GetOrElse(index, dictionary, action);
            if (result == CommandStatus.SUCCESS && AnalogCommandAccepted != null)
            {
                AnalogCommandAccepted(value, index);
            }
            return result;
        }

        CommandStatus ICommandHandler.Supports(ControlRelayOutputBlock command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElse(index, binaryMap, () => proxy.Supports(command, index));
            }
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputInt32 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElse(index, analogMap, () => proxy.Supports(command, index));
            }
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputInt16 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElse(index, analogMap, () => proxy.Supports(command, index));
            }
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputFloat32 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElse(index, analogMap, () => proxy.Supports(command, index));
            }
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputDouble64 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElse(index, analogMap, () => proxy.Supports(command, index));
            }
        }

        CommandStatus ICommandHandler.Perform(ControlRelayOutputBlock command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElseAndLogBinary(command, index, binaryMap, () => proxy.Perform(command, index));
            }
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputInt32 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElseAndLogAnalog(command.value, index, binaryMap, () => proxy.Perform(command, index));
            }
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputInt16 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElseAndLogAnalog(command.value, index, binaryMap, () => proxy.Perform(command, index));
            }
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputFloat32 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElseAndLogAnalog(command.value, index, binaryMap, () => proxy.Perform(command, index));
            }
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputDouble64 command, ushort index)
        {
            lock (mutex)
            {
                return GetOrElseAndLogAnalog(command.value, index, binaryMap, () => proxy.Perform(command, index));
            }
        }
    }
}

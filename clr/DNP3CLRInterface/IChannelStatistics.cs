﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
   public interface IChannelStatistics
   {
       /// <summary>
       /// The number of times the channel has sucessfully opened
       /// </summary>
       System.UInt32 NumOpen { get; }

       /// <summary>
       /// The number of times the channel has failed to open
       /// </summary>
       System.UInt32 NumOpenFail { get; }

       /// <summary>
       /// The number of times the channel has closed either due to user intervention or an error
       /// </summary>
       System.UInt32 NumClose { get; }

       /// <summary>
       /// The number of bytes received
       /// </summary>
       System.UInt32 NumBytesRx { get; }

       /// <summary>
       /// The number of bytes transmitted
       /// </summary>
       System.UInt32 NumNytesTx { get; }

       /// <summary>
       /// Number of frames discared due to CRC errors
       /// </summary>
       System.UInt32 NumCrcError { get; }

       /// <summary>
       /// Number of frames detected with bad / malformed contents
       /// </summary>
       System.UInt32 NumBadLinkFrame { get; }      
   }
}

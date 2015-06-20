/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "TransportRx.h"
#include "TransportConstants.h"
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/util/ToHex.h>

#include <cstring>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportRx::TransportRx(const Logger& logger_, uint32_t maxRxFragSize, StackStatistics* pStatistics_) :
	logger(logger_),	
	pStatistics(pStatistics_),
	rxBuffer(maxRxFragSize),	
	numBytesRead(0)	
{
	
}

void TransportRx::Reset()
{
	sequence.Reset();
	this->ClearRxBuffer();	
}

void TransportRx::ClearRxBuffer()
{
	numBytesRead = 0;
}

openpal::WriteBufferView TransportRx::GetAvailable()
{
	return rxBuffer.GetWriteBufferView().Skip(numBytesRead);
}

ReadBufferView TransportRx::ProcessReceive(const ReadBufferView& input)
{
	if (input.Size() == 0)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_NO_HEADER, "Received tpdu with no header");
		if (pStatistics) ++pStatistics->numTransportErrorRx;
		return ReadBufferView::Empty();
	}	
	else
	{
		uint8_t hdr = input[0];
		bool first = (hdr & TL_HDR_FIR) != 0;
		bool last = (hdr & TL_HDR_FIN) != 0;
		int seq = hdr & TL_HDR_SEQ;

		auto payload = input.Skip(1);		

		FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_RX, "FIR: %d FIN: %d SEQ: %u LEN: %u", first, last, seq, payload.Size());

		if (!this->ValidateHeader(first, last, seq))
		{
			if (pStatistics) ++pStatistics->numTransportErrorRx;
			return ReadBufferView::Empty();
		}

		auto available = this->GetAvailable();

		if (payload.Size() > available.Size())
		{
			if (pStatistics) ++pStatistics->numTransportErrorRx;
			SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_BUFFER_FULL, "Exceeded the buffer size before a complete fragment was read");
			this->ClearRxBuffer();
			return ReadBufferView::Empty();
		}
		else   //passed all validation
		{
			if (pStatistics) ++pStatistics->numTransportRx;

			auto payload = input.Skip(1);
				
			payload.CopyTo(available);

			this->numBytesRead += payload.Size();
			this->sequence.Increment();

			if(last)
			{			
				ReadBufferView ret = rxBuffer.ToReadOnly().Take(numBytesRead);					
				this->ClearRxBuffer();
				return ret;
			}
			else
			{
				return ReadBufferView::Empty();
			}
		}		
	}


}

bool TransportRx::ValidateHeader(bool fir, bool fin, uint8_t sequence_)
{	
	if(fir)
	{
		sequence = sequence_; //always accept the sequence on FIR
		if (numBytesRead > 0)
		{
			// drop existing received bytes from segment
			SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_NEW_FIR_MID_SEQUENCE, "FIR received mid-fragment, discarding previous bytes");
			numBytesRead = 0;
		}
		return true;
	}
	
	if(numBytesRead == 0)   //non-first packet with 0 prior bytes
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_MESSAGE_WITHOUT_FIR, "non-FIR packet with 0 prior bytes");
		return false;
	}

	if(sequence_ != sequence)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_BAD_SEQUENCE, "Ignoring bad sequence, got %u, expected %u", sequence_, sequence.Get());
		return false;
	}

	return true;
}

}


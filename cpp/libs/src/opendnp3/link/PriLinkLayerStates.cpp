/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "PriLinkLayerStates.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/link/LinkLayer.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

PriStateBase& PriStateBase::OnAck(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnNack(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnLinkStatus(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnNotSupported(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnTxReady(LinkContext& ctx)
{
    FORMAT_LOG_BLOCK(ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());
    return *this;
}

PriStateBase& PriStateBase::OnTimeout(LinkContext& ctx)
{
    FORMAT_LOG_BLOCK(ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());
    return *this;
}

PriStateBase& PriStateBase::TrySendConfirmed(LinkContext& /*ctx*/, ITransportSegment& /*unused*/)
{
    return *this;
}

PriStateBase& PriStateBase::TrySendUnconfirmed(LinkContext& /*ctx*/, ITransportSegment& /*unused*/)
{
    return *this;
}

PriStateBase& PriStateBase::TrySendRequestLinkStatus(LinkContext& /*unused*/)
{
    return *this;
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_Idle PLLS_Idle::instance;

PriStateBase& PLLS_Idle::TrySendUnconfirmed(LinkContext& ctx, ITransportSegment& segments)
{
    auto first = segments.GetSegment();
    auto output = ctx.FormatPrimaryBufferWithUnconfirmed(segments.GetAddresses(), first);
    ctx.QueueTransmit(output, true);
    return PLLS_SendUnconfirmedTransmitWait::Instance();
}

PriStateBase& PLLS_Idle::TrySendConfirmed(LinkContext& ctx, ITransportSegment& segments)
{
    if (ctx.isRemoteReset)
    {
        ctx.ResetRetry();
        auto buffer
            = ctx.FormatPrimaryBufferWithConfirmed(segments.GetAddresses(), segments.GetSegment(), ctx.nextWriteFCB);
        ctx.QueueTransmit(buffer, true);
        return PLLS_ConfUserDataTransmitWait::Instance();
    }

    ctx.ResetRetry();
    ctx.QueueResetLinks(segments.GetAddresses().destination);
    return PLLS_LinkResetTransmitWait::Instance();
}

PriStateBase& PLLS_Idle::TrySendRequestLinkStatus(LinkContext& ctx)
{
    ctx.keepAliveTimeout = false;
    ctx.QueueRequestLinkStatus(ctx.config.RemoteAddr);
    ctx.listener->OnKeepAliveInitiated();
    return PLLS_RequestLinkStatusTransmitWait::Instance();
}

////////////////////////////////////////////////////////
//	Class SendUnconfirmedTransmitWait
////////////////////////////////////////////////////////

PLLS_SendUnconfirmedTransmitWait PLLS_SendUnconfirmedTransmitWait::instance;

PriStateBase& PLLS_SendUnconfirmedTransmitWait::OnTxReady(LinkContext& ctx)
{
    if (ctx.pSegments->Advance())
    {
        auto output
            = ctx.FormatPrimaryBufferWithUnconfirmed(ctx.pSegments->GetAddresses(), ctx.pSegments->GetSegment());
        ctx.QueueTransmit(output, true);
        return *this;
    }
    // we're done

    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::instance;

PriStateBase& PLLS_LinkResetTransmitWait::OnTxReady(LinkContext& ctx)
{
    // now we're waiting for an ACK
    ctx.StartResponseTimer();
    return PLLS_ResetLinkWait::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::instance;

PriStateBase& PLLS_ConfUserDataTransmitWait::OnTxReady(LinkContext& ctx)
{
    // now we're waiting on an ACK
    ctx.StartResponseTimer();
    return PLLS_ConfDataWait::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the request link status
/////////////////////////////////////////////////////////////////////////////

PLLS_RequestLinkStatusTransmitWait PLLS_RequestLinkStatusTransmitWait::instance;

PriStateBase& PLLS_RequestLinkStatusTransmitWait::OnTxReady(LinkContext& ctx)
{
    // now we're waiting on a LINK_STATUS
    ctx.StartResponseTimer();
    return PLLS_RequestLinkStatusWait::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::instance;

PriStateBase& PLLS_ResetLinkWait::OnAck(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ctx.isRemoteReset = true;
    ctx.ResetWriteFCB();
    ctx.CancelTimer();
    auto buffer = ctx.FormatPrimaryBufferWithConfirmed(ctx.pSegments->GetAddresses(), ctx.pSegments->GetSegment(),
                                                       ctx.nextWriteFCB);
    ctx.QueueTransmit(buffer, true);
    ctx.listener->OnStateChange(opendnp3::LinkStatus::RESET);
    return PLLS_ConfUserDataTransmitWait::Instance();
}

PriStateBase& PLLS_ResetLinkWait::OnTimeout(LinkContext& ctx)
{
    if (ctx.Retry())
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN, "Link reset timeout, retrying %i remaining", ctx.numRetryRemaining);
        ctx.QueueResetLinks(ctx.config.RemoteAddr);
        return PLLS_LinkResetTransmitWait::Instance();
    }

    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Link reset final timeout, no retries remain");
    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_ResetLinkWait::Failure(LinkContext& ctx)
{
    ctx.CancelTimer();
    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::instance;

PriStateBase& PLLS_ConfDataWait::OnAck(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ctx.ToggleWriteFCB();
    ctx.CancelTimer();

    if (ctx.pSegments->Advance())
    {
        auto buffer = ctx.FormatPrimaryBufferWithConfirmed(ctx.pSegments->GetAddresses(), ctx.pSegments->GetSegment(),
                                                           ctx.nextWriteFCB);
        ctx.QueueTransmit(buffer, true);
        return PLLS_ConfUserDataTransmitWait::Instance();
    }
    // we're done!

    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_ConfDataWait::OnNack(LinkContext& ctx, bool rxBuffFull)
{
    ctx.listener->OnStateChange(opendnp3::LinkStatus::UNRESET);

    if (rxBuffFull)
    {
        return Failure(ctx);
    }

    ctx.ResetRetry();
    ctx.CancelTimer();
    ctx.QueueResetLinks(ctx.pSegments->GetAddresses().destination);
    return PLLS_LinkResetTransmitWait::Instance();
}

PriStateBase& PLLS_ConfDataWait::Failure(LinkContext& ctx)
{
    ctx.CancelTimer();
    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_ConfDataWait::OnTimeout(LinkContext& ctx)
{
    if (ctx.Retry())
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN, "confirmed data timeout, retrying %u remaining",
                         ctx.numRetryRemaining);
        auto buffer = ctx.FormatPrimaryBufferWithConfirmed(ctx.pSegments->GetAddresses(), ctx.pSegments->GetSegment(),
                                                           ctx.nextWriteFCB);
        ctx.QueueTransmit(buffer, true);
        return PLLS_ConfUserDataTransmitWait::Instance();
    }

    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Confirmed data final timeout, no retries remain");
    ctx.listener->OnStateChange(opendnp3::LinkStatus::UNRESET);
    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_RequestLinkStatusWait
////////////////////////////////////////////////////////

PLLS_RequestLinkStatusWait PLLS_RequestLinkStatusWait::instance;

PriStateBase& PLLS_RequestLinkStatusWait::OnNack(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.FailKeepAlive(false);
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnLinkStatus(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.CompleteKeepAlive();
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnNotSupported(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.FailKeepAlive(false);
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnTimeout(LinkContext& ctx)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Link status request - response timeout");
    ctx.FailKeepAlive(true);
    return PLLS_Idle::Instance();
}

} // namespace opendnp3

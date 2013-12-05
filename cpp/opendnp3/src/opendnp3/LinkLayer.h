#ifndef __LINK_LAYER_H_
#define __LINK_LAYER_H_

#include <queue>

#include <openpal/Visibility.h>
#include <openpal/AsyncLayerInterfaces.h>
#include <openpal/IExecutor.h>

#include "ILinkContext.h"
#include "LinkFrame.h"

#include <opendnp3/LinkConfig.h>

namespace opendnp3
{

class ILinkRouter;
class PriStateBase;
class SecStateBase;

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class DLL_LOCAL LinkLayer : public openpal::ILowerLayer, public ILinkContext
{
public:

	LinkLayer(openpal::Logger, openpal::IExecutor*, const LinkConfig& arConfig);

	void SetRouter(ILinkRouter*);

	// ILinkContext interface
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	// IFrameSink interface
	void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer);
	void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer);

	// Functions called by the primary and secondary station states
	void ChangeState(PriStateBase*);
	void ChangeState(SecStateBase*);

	openpal::Logger& GetLogger() {
		return mLogger;
	}

	void DoDataUp(const openpal::ReadOnlyBuffer& arBuffer) {
		if(mpUpperLayer) mpUpperLayer->OnReceive(arBuffer);
	}

	void DoSendSuccess() {
		if(mpUpperLayer) mpUpperLayer->OnSendSuccess();
	}

	void DoSendFailure() {
		if(mpUpperLayer) mpUpperLayer->OnSendFailure();
	}

	void ResetReadFCB() {
		mNextReadFCB = true;
	}
	void ToggleReadFCB() {
		mNextReadFCB = !mNextReadFCB;
	}
	bool NextReadFCB() {
		return mNextReadFCB;
	}

	void ResetWriteFCB() {
		mNextWriteFCB = true;
	}
	void ToggleWriteFCB() {
		mNextWriteFCB = !mNextWriteFCB;
	}
	bool NextWriteFCB() {
		return mNextWriteFCB;
	}

	// Helpers for sending frames
	void SendAck();
	void SendLinkStatus();
	void SendResetLinks();
	void SendUnconfirmedUserData(const openpal::ReadOnlyBuffer&);
	void SendDelayedUserData(bool aFCB);

	void StartTimer();
	void CancelTimer();

	const LinkConfig mCONFIG;

	//Retry Count
	void ResetRetry();
	bool Retry();
	size_t RetryRemaining() {
		return mRetryRemaining;
	}

	LinkFrame mPriFrame;
	LinkFrame mSecFrame;
	LinkFrame mDelayedPriFrame;

private:

	void Transmit(const LinkFrame&);

	size_t mRetryRemaining;

	openpal::IExecutor* mpExecutor;
	openpal::ITimer* mpTimer;

	// callback from the active timer
	void OnTimeout();

	bool mNextReadFCB;
	bool mNextWriteFCB;
	bool mIsOnline;

	bool Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest);

	/* Events - NVII delegates from ILayerDown and Events produced internally */
	void _Send(const openpal::ReadOnlyBuffer& arBuffer);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string SendString() {
		return "~>";
	}
#endif

	ILinkRouter* mpRouter;
	PriStateBase* mpPriState;
	SecStateBase* mpSecState;
};

}

#endif


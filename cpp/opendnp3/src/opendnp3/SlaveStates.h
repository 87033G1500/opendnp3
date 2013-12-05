#ifndef __SLAVE_STATES_H_
#define __SLAVE_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <openpal/Visibility.h>

#include "AppInterfaces.h"

namespace opendnp3
{

class ITaskCompletion;
class Slave;
class APDU;

/**
 * Base class for all transaction states of a Slave
 */
class DLL_LOCAL AS_Base
{
public:

	/* Events from application layer */

	virtual void OnLowerLayerUp(Slave*);
	virtual void OnLowerLayerDown(Slave*);

	virtual void OnSolSendSuccess(Slave*);
	virtual void OnSolFailure(Slave*);

	virtual void OnUnsolSendSuccess(Slave*);
	virtual void OnUnsolFailure(Slave*);

	virtual void OnRequest(Slave*, const APDU&, SequenceInfo);

	virtual void OnUnknown(Slave*);

	/* Events produced from the user layer */

	// Called when a data update is received from the user layer
	virtual void OnDataUpdate(Slave*);

	// Called when a data update is received from the user layer
	virtual void OnUnsolExpiration(Slave*);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	// @return The name associated with the state
	virtual std::string Name() const = 0;
#endif

	virtual bool AcceptsDeferredRequests() {
		return false;
	}
	virtual bool AcceptsDeferredUpdates()  {
		return false;
	}
	virtual bool AcceptsDeferredUnsolExpiration()  {
		return false;
	}
	virtual bool AcceptsDeferredUnknown()  {
		return false;
	}

protected:

	void SwitchOnFunction(Slave*, AS_Base* apNext, const APDU& arRequest, SequenceInfo aSeqInfo);
	void DoUnsolSuccess(Slave*);
	void DoRequest(Slave* c, AS_Base* apNext, const APDU& arAPDU, SequenceInfo aSeqInfo);

	//Work functions

	void ChangeState(Slave*, AS_Base*);

};

/**
 * A state that indicates when the application layer has not informed the
 * slave yet that it is up.
 */
class DLL_LOCAL AS_Closed : public AS_Base
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_Closed);

	void OnLowerLayerUp(Slave*);
	void OnDataUpdate(Slave*);

	bool AcceptsDeferUpdates() {
		return true;
	}

};

class DLL_LOCAL AS_OpenBase : public AS_Base
{
public:

	void OnLowerLayerDown(Slave*);

};

/**
 * A state that indicates when the AppLayer is online but the Slave is not
 * doing or waiting on anything.
 */
class DLL_LOCAL AS_Idle : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_Idle);

	void OnRequest(Slave*, const APDU&, SequenceInfo);
	void OnDataUpdate(Slave*);
	void OnUnsolExpiration(Slave*);
	void OnUnknown(Slave*);

	bool AcceptsDeferredRequests() {
		return true;
	}
	bool AcceptsDeferredUpdates() {
		return true;
	}
	bool AcceptsDeferredUnsolExpiration()  {
		return true;
	}

};


/**
 * A state that indicates when the Slave is waiting for a response to
 * complete.
 */
class DLL_LOCAL AS_WaitForRspSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForRspSuccess);

	void OnRequest(Slave*, const APDU&, SequenceInfo);
	void OnSolFailure(Slave*);
	void OnSolSendSuccess(Slave*);

};

/**
 * A state that indicates when the Slave is waiting for an unsolicited
 * response to complete.
 */
class DLL_LOCAL AS_WaitForUnsolSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForUnsolSuccess);

	void OnRequest(Slave*, const APDU&, SequenceInfo);
	void OnUnsolFailure(Slave*);
	void OnUnsolSendSuccess(Slave*);

};

/**
 * A state that indicates when the Slave is waiting for an unsolicited
 * response and a solicited response to complete.
 */
class DLL_LOCAL AS_WaitForSolUnsolSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForSolUnsolSuccess);

	void OnRequest(Slave*, const APDU&, SequenceInfo);
	void OnSolFailure(Slave*);
	void OnSolSendSuccess(Slave*);
	void OnUnsolFailure(Slave*);
	void OnUnsolSendSuccess(Slave*);

};

} //ens ns

/* vim: set ts=4 sw=4: */

#endif


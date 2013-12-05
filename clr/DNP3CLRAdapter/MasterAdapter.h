#ifndef __MASTER_ADAPTER_H_
#define __MASTER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IMaster.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class MasterAdapter : IMaster
{
public:

	MasterAdapter(opendnp3::IMaster* apMaster);

	virtual ICommandProcessor ^ GetCommandProcessor();

	virtual void Enable();

	virtual void Disable();
        
	virtual void Shutdown();

	virtual IMasterScan^ GetIntegrityScan();

	virtual void AddStateListener(System::Action<StackState> ^ aListener);

	virtual IMasterScan^ AddClassScan(int aClassMask, System::TimeSpan period, System::TimeSpan taskRetryPeriod);

private:
	opendnp3::IMaster* mpMaster;
	ICommandProcessor ^ mCommandAdapter;
};

}
}

#endif

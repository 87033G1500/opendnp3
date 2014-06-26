#ifndef __SCAN_LISTENER_ADAPTER_H_
#define __SCAN_LISTENER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

//#include <opendnp3/master/ITaskListener.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class ScanListenerAdapter// : public opendnp3::ITaskListener
{
public:

	ScanListenerAdapter(System::Action<ScanResult>^ callback) : myCallback(callback)
	{}

	/* TODO
	virtual void OnTaskUpdate(opendnp3::TaskLifecycle status) final
	{		
		ScanStatus status = (result.status == opendnp3::ScanStatus::SUCCESS) ? ScanStatus::SUCCESS : ScanStatus::FAILURE;
		myCallback->Invoke(ScanResult(status));	
	}
	*/

private:

	gcroot<System::Action<ScanResult>^> myCallback;
};

}
}

#endif

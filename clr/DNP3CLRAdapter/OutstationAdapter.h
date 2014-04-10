#ifndef __OUTSTATION_ADAPTER_H_
#define __OUTSTATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IOutstation.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private ref class OutstationAdapter : IOutstation
{
public:

	OutstationAdapter(asiodnp3::IOutstation* apOutstation);

	virtual DNP3::Interface::IMeasurementLoader^ GetLoader();

	virtual void SetNeedTimeIIN();

	virtual void Shutdown();

	virtual void Enable();

	virtual void Disable();

private:
	asiodnp3::IOutstation* mpOutstation;
	DNP3::Interface::IMeasurementLoader^ mLoaderAdapter;
};

}
}

#endif

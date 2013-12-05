#include "MasterDataObserverAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{
MasterDataObserverAdapter::MasterDataObserverAdapter(DNP3::Interface::IDataObserver ^ proxy) : proxy(proxy)
{}

void MasterDataObserverAdapter::_Start()
{
	proxy->Start();
}

void MasterDataObserverAdapter::_Update(const opendnp3::Binary& arPoint, size_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::_Update(const opendnp3::Analog& arPoint, size_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::_Update(const opendnp3::Counter& arPoint, size_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::_Update(const opendnp3::ControlStatus& arPoint, size_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::_Update(const opendnp3::SetpointStatus& arPoint, size_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::_End()
{
	proxy->End();
}

}
}



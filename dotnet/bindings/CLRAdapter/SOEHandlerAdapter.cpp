#include "SOEHandlerAdapter.h"

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{

SOEHandlerAdapter::SOEHandlerAdapter(DNP3::Interface::ISOEHandler^ aProxy) : proxy(aProxy)
{}

void SOEHandlerAdapter::Start()
{
	proxy->Start();
}

void SOEHandlerAdapter::End()
{
	proxy->End();
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas)
{
	this->DispatchStatic(meas);
}

void SOEHandlerAdapter::LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}

void SOEHandlerAdapter::LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas)
{
	this->DispatchEvent(meas);
}


}
}

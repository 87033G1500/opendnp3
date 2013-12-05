#include "IndexedWriteIterator.h"


#include "ObjectHeader.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3
{

IndexedWriteIterator::IndexedWriteIterator() :
	mpPos(nullptr),
	mIndexMode(IM_NONE),
	mIndex(0),
	mCount(0),
	mObjectSize(0),
	mIndexSet(false)
{

}

IndexedWriteIterator::IndexedWriteIterator(uint8_t* apPos, size_t aCount, QualifierCode aCode, size_t aObjectSize) :
	mpPos(apPos),
	mIndexMode(GetIndexMode(aCode)),
	mIndex(0),
	mCount(aCount),
	mObjectSize(aObjectSize),
	mIndexSet(false)
{

}

uint8_t* IndexedWriteIterator::operator*() const
{
	if(!mIndexSet) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Index has not been written");
	return mpPos + mIndexMode;
}

IndexedWriteIterator::IndexMode IndexedWriteIterator::GetIndexMode(QualifierCode aCode)
{
	switch(aCode) {
	case(QualifierCode::UINT8_CNT_UINT8_INDEX): return IM_1B;
	case(QualifierCode::UINT16_CNT_UINT16_INDEX): return IM_2B;
	case(QualifierCode::UINT32_CNT_UINT32_INDEX): return IM_4B;

	case(QualifierCode::UINT8_CNT):
	case(QualifierCode::UINT16_CNT):
	case(QualifierCode::UINT32_CNT):
		return IM_NONE;

	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Illegal qualifer for packed indexed");
	}
}

void IndexedWriteIterator::SetIndex(size_t aIndex)
{
	if(mIndexSet) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Index already set");
	if(IsEnd()) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "End of iteration");

	switch(mIndexMode) {
	case(IM_1B):
		if(aIndex > UInt8::Max) MACRO_THROW_EXCEPTION(openpal::ArgumentException , "");
		UInt8::Write(mpPos, static_cast<uint8_t>(aIndex));
		break;
	case(IM_2B):
		if(aIndex > UInt16::Max) MACRO_THROW_EXCEPTION(openpal::ArgumentException, "");
		UInt16::Write(mpPos, static_cast<uint16_t>(aIndex));
		break;
	case(IM_4B):
		if(aIndex > UInt32::Max) MACRO_THROW_EXCEPTION(openpal::ArgumentException, "");
		UInt32::Write(mpPos, static_cast<uint32_t>(aIndex));
		break;
	default:
		break;
	}

	mIndexSet = true;
}

const IndexedWriteIterator& IndexedWriteIterator::operator++()
{
	if(this->IsEnd()) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "End of iterattion");
	if(!mIndexSet) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Index has not been set");

	++mIndex;
	mIndexSet = false;
	mpPos += mObjectSize + mIndexMode;

	return (*this);
}

const IndexedWriteIterator IndexedWriteIterator::operator++(int)
{
	IndexedWriteIterator tmp(*this);
	++(*this);
	return tmp;
}

}


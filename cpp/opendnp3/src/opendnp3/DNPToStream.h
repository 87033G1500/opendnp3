#ifndef __DNP_TO_STREAM_H_
#define __DNP_TO_STREAM_H_

#include <opendnp3/DataTypes.h>
#include <openpal/Visibility.h>

#ifdef WIN32
//disable the type converstion warnings
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#endif

namespace opendnp3
{

class DLL_LOCAL DNPToStream
{
public:

	// Templates for writing to stream
	template <typename T, typename U>
	static void WriteQ(uint8_t* apPos, const T* apObj, const U& arObj);

	template <typename T, typename U>
	static void WriteQV(uint8_t* apPos, const T* apObj, const U& arObj);

	template <typename T, typename U>
	static void WriteQT(uint8_t* apPos, const T* apObj, const U& arObj);

	template <typename T, typename U>
	static void WriteCheckRangeQV(uint8_t* apPos, const T* apObj, const U&  arObj);

	template <typename T, typename U>
	static void WriteV(uint8_t* apPos, const T* apObj, const U&  arObj);

	template<typename T, typename U>
	static void WriteQVT(uint8_t* apPos, const T* apObj, const U&  arObj);

	template<typename T, typename U>
	static void WriteCheckRangeQVT(uint8_t* apPos, const T* apObj, const U&  arObj);
};

template <class T, class U>
class Conversion
{
public:
	static U Convert(T aVal) {
		return aVal;    //try implicit casting
	}
};

//this partial specialization allows us to change how doubles are converted
template <class U>
class Conversion<double, U>
{
public:
	static U Convert(double aVal) {
		return static_cast<U>(aVal);
	}
};

template <typename T, typename U>
inline void DNPToStream::WriteQ(uint8_t* apPos, const T* apObj, const U& arObj)
{
	apObj->mFlag.Set(apPos, arObj.GetQuality());
}

template <typename T, typename U>
inline void DNPToStream::WriteQT(uint8_t* apPos, const T* apObj, const U& arObj)
{
	apObj->mFlag.Set(apPos, arObj.GetQuality());
	apObj->mTime.Set(apPos, arObj.GetTime());
}

// Templates for writing to stream
template <typename T, typename U>
void DNPToStream::WriteCheckRangeQV(uint8_t* apPos, const T* apObj, const U& arObj)
{
	uint8_t qual = arObj.GetQuality();

	typename U::Type val = arObj.GetValue();
	if(val > apObj->mValue.Max()) {
		val = apObj->mValue.Max();
		qual |= apObj->mValue.OverRangeMask();
	}
	if(val < apObj->mValue.Min()) {
		val = apObj->mValue.Min();
		qual |= apObj->mValue.OverRangeMask();
	}

	apObj->mFlag.Set(apPos, qual);
	apObj->mValue.Set(apPos, Conversion<typename U::Type, typename T::Type>::Convert(val));
}

// Templates for writing to stream
template <typename T, typename U>
void DNPToStream::WriteV(uint8_t* apPos, const T* apObj, const U& arObj)
{
	typename U::Type val = arObj.GetValue();
	if(val > apObj->mValue.Max()) {
		val = apObj->mValue.Max();
	}
	if(val < apObj->mValue.Min()) {
		val = apObj->mValue.Min();
	}

	apObj->mValue.Set(apPos, Conversion<typename U::Type, typename T::Type>::Convert(val));
}

// Templates for writing to stream
template <typename T, typename U>
inline void DNPToStream::WriteQV(uint8_t* apPos, const T* apObj, const U& arObj)
{
	apObj->mFlag.Set(apPos, arObj.GetQuality());
	WriteV<T, U>(apPos, apObj, arObj);
}

template<typename T, typename U>
void DNPToStream::WriteQVT(uint8_t* apPos, const T* apObj, const U&  arObj)
{
	WriteQV<T, U>(apPos, apObj, arObj);
	apObj->mTime.Set(apPos, arObj.GetTime());
}

template<typename T, typename U>
inline void DNPToStream::WriteCheckRangeQVT(uint8_t* apPos, const T* apObj, const U& arObj)
{
	WriteCheckRangeQV<T, U>(apPos, apObj, arObj);
	apObj->mTime.Set(apPos, arObj.GetTime());
}

}

#ifdef WIN32
//disable the type converstion warnings
#pragma warning ( pop )
#endif

#endif


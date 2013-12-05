#ifndef __ANALOG_OUTPUT_H_
#define __ANALOG_OUTPUT_H_

#include "gen/CommandStatus.h"

namespace opendnp3
{

/**
 * The object to represent a setpoint request from the master. Think of
 * this like turning a dial on the front of a machine to desired setting.
 */
template <class T>
class AnalogOutput
{
public:

	/**
	 * Creates a new instance with underlying type T
	*/
	AnalogOutput(T aValue) :
		mValue(aValue),
		mStatus(CommandStatus::SUCCESS)
	{}

	AnalogOutput() :
		mValue(0),
		mStatus(CommandStatus::SUCCESS)
	{}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string ToString() const = 0;
#endif

	/**
	* @return the underlying value type
	*/
	T GetValue() const {
		return mValue;
	}

public:

	/**
	* The status value defaults to CS_SUCCESS for requests
	*/
	CommandStatus mStatus;

protected:
	T mValue;
};

/**
*	16-bit signed integer analog output. The underlying serialization is Group41, Variation 2
*/
class AnalogOutputInt16 : public AnalogOutput<int16_t>
{
public:

	AnalogOutputInt16();
	AnalogOutputInt16(int16_t);

	bool operator==(const AnalogOutputInt16& arRHS) const;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};

/**
*	32-bit signed integer analog output. The underlying serialization is Group41, Variation 1
*/
class AnalogOutputInt32 : public AnalogOutput<int32_t>
{
public:

	AnalogOutputInt32();
	AnalogOutputInt32(int32_t);

	bool operator==(const AnalogOutputInt32& arRHS) const;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};

/**
*	Single precision analog output. The underlying serialization is Group41, Variation 3
*/
class AnalogOutputFloat32 : public AnalogOutput<float>
{
public:

	AnalogOutputFloat32();
	AnalogOutputFloat32(float);

	bool operator==(const AnalogOutputFloat32& arRHS) const;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};

/**
*	Double precision analog output. The underlying serialization is Group41, Variation 3
*/
class AnalogOutputDouble64 : public AnalogOutput<double>
{
public:

	AnalogOutputDouble64();
	AnalogOutputDouble64(double);

	bool operator==(const AnalogOutputDouble64& arRHS) const;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};


}

/* vim: set ts=4 sw=4: */

#endif

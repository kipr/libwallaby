/*
 * accel.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ACCEL_HPP_
#define INCLUDE_WALLABY_ACCEL_HPP_

#include "sensor.hpp"

class Acceleration
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
};

class AccelX : public Sensor<short>
{
public:
	virtual short value() const;
};

class AccelY : public Sensor<short>
{
public:
	virtual short value() const;
};

class AccelZ : public Sensor<short>
{
public:
	virtual short value() const;
};

#endif /* INCLUDE_WALLABY_ACCEL_HPP_ */

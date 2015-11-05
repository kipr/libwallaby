/*
 * accel.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/accel.hpp"
#include "accel_p.hpp"


signed short Acceleration::x()
{
	return Private::Accel::instance()->xAccel();
}

signed short Acceleration::y()
{
	return Private::Accel::instance()->yAccel();
}

signed short Acceleration::z()
{
	return Private::Accel::instance()->zAccel();
}

bool Acceleration::calibrate()
{
	return Private::Accel::instance()->calibrate();
}

short AccelX::value() const
{
	return Private::Accel::instance()->xAccel();
}

short AccelY::value() const
{
	return Private::Accel::instance()->yAccel();
}

short AccelZ::value() const
{
	return Private::Accel::instance()->zAccel();
}

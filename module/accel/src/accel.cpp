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
	return Private::accel_x();
}

signed short Acceleration::y()
{
	return Private::accel_y();
}

signed short Acceleration::z()
{
	return Private::accel_z();
}

bool Acceleration::calibrate()
{
	return Private::accel_calibrate();
}

short AccelX::value() const
{
	return Private::accel_x();
}

short AccelY::value() const
{
	return Private::accel_y();
}

short AccelZ::value() const
{
	return Private::accel_z();
}

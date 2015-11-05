/*
 * accel_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/accel.h"
#include "accel_p.hpp"

short accel_x()
{
	return Private::Accel::instance()->xAccel();
}

short accel_y()
{
	return Private::Accel::instance()->yAccel();
}

short accel_z()
{
	return Private::Accel::instance()->zAccel();
}

int accel_calibrate()
{
	return Private::Accel::instance()->calibrate() ? 0 : -1;
}

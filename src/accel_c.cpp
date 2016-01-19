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
	return Private::accel_x();
}

short accel_y()
{
	return Private::accel_y();
}

short accel_z()
{
	return Private::accel_z();
}

int accel_calibrate()
{
	return Private::accel_calibrate();
}

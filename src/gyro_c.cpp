/*
 * gyro_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/gyro.h"
#include "gyro_p.hpp"

short gyro_x()
{
	return Private::gyro_x();
}

short gyro_y()
{
	return Private::gyro_y();
}

short gyro_z()
{
	return Private::gyro_z();
}

int gyro_calibrate()
{
	return (Private::gyro_calibrate() ? 0 : -1);
}

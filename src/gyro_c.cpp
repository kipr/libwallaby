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
	return Private::Gyro::instance()->xGyro();
}

short gyro_y()
{
	return Private::Gyro::instance()->yGyro();
}

short gyro_z()
{
	return Private::Gyro::instance()->zGyro();
}

int gyro_calibrate()
{
	return Private::Gyro::instance()->calibrate() ? 0 : -1;
}

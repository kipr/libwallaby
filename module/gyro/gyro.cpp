/*
 * gyro.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/gyro.hpp"
#include "gyro_p.hpp"

signed short Gyro::x()
{
	return Private::gyro_x();
}

signed short Gyro::y()
{
	return Private::gyro_y();
}

signed short Gyro::z()
{
	return Private::gyro_z();
}

bool Gyro::calibrate()
{
	return Private::gyro_calibrate();
}

short GyroX::value() const
{
	return Private::gyro_x();
}

short GyroY::value() const
{
	return Private::gyro_y();
}

short GyroZ::value() const
{
	return Private::gyro_z();
}

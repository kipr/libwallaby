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
	return Private::Gyro::instance()->xGyro();
}

signed short Gyro::y()
{
	return Private::Gyro::instance()->yGyro();
}

signed short Gyro::z()
{
	return Private::Gyro::instance()->zGyro();
}

bool Gyro::calibrate()
{
	return Private::Gyro::instance()->calibrate();
}

short GyroX::value() const
{
	return Private::Gyro::instance()->xGyro();
}

short GyroY::value() const
{
	return Private::Gyro::instance()->yGyro();
}

short GyroZ::value() const
{
	return Private::Gyro::instance()->zGyro();
}

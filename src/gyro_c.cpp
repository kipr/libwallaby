/*
 * gyro_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/gyro.h"
#include "wallaby_p.hpp"

short gyro_x()
{
	return Private::BattleHill::instance()->getGyroX();
}

short gyro_y()
{
	return Private::BattleHill::instance()->getGyroY();
}

short gyro_z()
{
	return Private::BattleHill::instance()->getGyroZ();
}

int gyro_calibrate()
{
	return Private::BattleHill::instance()->calibrateGyro() ? 0 : -1;
}

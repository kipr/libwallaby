/*
 * accel_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/accel.h"
#include "battlehill_p.hpp"


short accel_x()
{
	return Private::BattleHill::instance()->getAccelX();
}

short accel_y()
{
	return Private::BattleHill::instance()->getAccelY();
}

short accel_z()
{
	return Private::BattleHill::instance()->getAccelZ();
}

int accel_calibrate()
{
	return  Private::BattleHill::instance()->calibrateAccel() ? 0 : -1;
}

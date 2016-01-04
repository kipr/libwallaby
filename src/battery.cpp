/*
 * battery.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/battery.hpp"

#include "battlehill_p.hpp"

bool Battery::isCharging()
{
	return false; // this board doesn't support charging
}


float Battery::powerLevel()
{
	return Private::BattleHill::instance()->getBatteryCapacity();
}

/*
 * battery.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/battery.hpp"

#include "battery_p.hpp"

bool Battery::isCharging()
{
	return false; // this board doesn't support charging
}


float Battery::powerLevel(unsigned int battery_type)
{
	return Private::battery_power_level(static_cast<Private::Battery::BatteryType>(battery_type));
}

unsigned short Battery::rawPowerADC()
{
	return Private::battery_raw_reading();
}

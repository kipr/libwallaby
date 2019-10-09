/*
 * battery.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_BATTERY_HPP_
#define INCLUDE_WALLABY_BATTERY_HPP_


class Battery
{
public:
	static bool isCharging();
	static float powerLevel(unsigned int battery_type = 0);
	static unsigned short rawPowerADC();
};

#endif /* INCLUDE_WALLABY_BATTERY_HPP_ */

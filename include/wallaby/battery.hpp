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
	static float powerLevel();
};

#endif /* INCLUDE_WALLABY_BATTERY_HPP_ */

/*
 * robot.hpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ROBOT_HPP_
#define INCLUDE_WALLABY_ROBOT_HPP_

class Robot
{
public:
	static unsigned long int getRobotStatesSequenceNumber();
	static unsigned long int getRobotUpdateCount();

	static unsigned short getRobotFirmwareVersion();

	static bool setRobotUpdateDelay(unsigned int us_delay);
	static unsigned int getRobotUpdateDelay();

	static bool setLowVoltThreshold(float volts);
	static float getLowVoltThreshold();
};



#endif /* INCLUDE_WALLABY_ROBOT_HPP_ */

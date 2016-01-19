/*
 * robot_c.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */


#include "wallaby/robot.h"
#include "wallaby/robot.hpp"
#include "wallaby_p.hpp"

unsigned long int get_robot_states_sequence_num()
{
	return 0;// TODO return Robot::getRobotStatesSequenceNumber();
}

unsigned long int get_robot_update_count()
{
	return Private::Wallaby::instance()->getUpdateCount();
}

unsigned short get_robot_firmware_version()
{
	return Private::Wallaby::instance()->getUpdateCount();
}

int set_robot_update_delay(unsigned int us_delay)
{
	return 0;// TODO return Robot::setRobotUpdateDelay(us_delay) ? 1 : 0;
}

unsigned int get_robot_update_delay()
{
	return 0;// TODO return Robot::getRobotUpdateDelay();
}

int set_low_volt_threshold(float volts)
{
	return 0;// TODO return Robot::setLowVoltThreshold(volts) ? 1 : 0;
}

float get_low_volt_threshold()
{
	return 0;// TODO return Robot::getLowVoltThreshold();
}

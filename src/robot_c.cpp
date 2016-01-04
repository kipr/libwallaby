/*
 * robot_c.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */


#include "wallaby/robot.h"
#include "wallaby/robot.hpp"

unsigned long int get_robot_states_sequence_num()
{
	return Robot::getRobotStatesSequenceNumber();
}

unsigned long int get_robot_update_count()
{
	return Robot::getRobotUpdateCount();
}

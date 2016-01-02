/*
 * robot.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "wallaby/robot.hpp"
#include "battlehill_p.hpp"

unsigned long int Robot::getRobotStatesSequenceNumber()
{
	return Private::BattleHill::instance()->getRobotStatesSequenceNumber();
}

unsigned long int Robot::getRobotUpdateCount()
{
	return Private::BattleHill::instance()->getRobotUpdateCount();
}

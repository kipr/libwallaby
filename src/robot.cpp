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

bool Robot::setRobotUpdateDelay(unsigned int us_delay)
{
	return Private::BattleHill::instance()->setRobotUpdateDelay(us_delay);
}

unsigned int Robot::getRobotUpdateDelay()
{
	return Private::BattleHill::instance()->getRobotUpdateDelay();
}

bool Robot::setLowVoltThreshold(float volts)
{
	return Private::BattleHill::instance()->setLowVoltThreshold(volts);
}

float Robot::getLowVoltThreshold()
{
	return Private::BattleHill::instance()->getLowVoltThreshold();
}

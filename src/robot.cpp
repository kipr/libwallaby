/*
 * robot.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "wallaby/robot.hpp"
#include "wallaby_p.hpp"

unsigned long int Robot::getRobotStatesSequenceNumber()
{
	return 0;// TODO return Private::BattleHill::instance()->getRobotStatesSequenceNumber();
}

unsigned long int Robot::getRobotUpdateCount()
{
	return Private::Wallaby::instance()->getUpdateCount();
}

unsigned short Robot::getRobotFirmwareVersion()
{
	return Private::Wallaby::instance()->getFirmwareVersion();
}

bool Robot::setRobotUpdateDelay(unsigned int us_delay)
{
	return false;// TODO return Private::BattleHill::instance()->setRobotUpdateDelay(us_delay);
}

unsigned int Robot::getRobotUpdateDelay()
{
	return 0;// TODO return Private::BattleHill::instance()->getRobotUpdateDelay();
}

bool Robot::setLowVoltThreshold(float volts)
{
	return false;// TODO return Private::BattleHill::instance()->setLowVoltThreshold(volts);
}

float Robot::getLowVoltThreshold()
{
	return 0;// TODO return Private::BattleHill::instance()->getLowVoltThreshold();
}

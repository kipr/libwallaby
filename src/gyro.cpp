/*
 * gyro.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/gyro.hpp"
#include "battlehill_p.hpp"

signed short Gyro::x()
{
	return Private::BattleHill::instance()->getGyroX();
}

signed short Gyro::y()
{
	return Private::BattleHill::instance()->getGyroY();
}

signed short Gyro::z()
{
	return Private::BattleHill::instance()->getGyroZ();
}

bool Gyro::calibrate()
{
	return Private::BattleHill::instance()->calibrateGyro();
}

short GyroX::value() const
{
	return Private::BattleHill::instance()->getGyroX();
}

short GyroY::value() const
{
	return Private::BattleHill::instance()->getGyroY();
}

short GyroZ::value() const
{
	return Private::BattleHill::instance()->getGyroZ();
}

/*
 * accel.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/accel.hpp"
#include "wallaby_p.hpp"


signed short Acceleration::x()
{
	return Private::BattleHill::instance()->getAccelX();
}

signed short Acceleration::y()
{
	return Private::BattleHill::instance()->getAccelY();
}

signed short Acceleration::z()
{
	return Private::BattleHill::instance()->getAccelZ();
}

bool Acceleration::calibrate()
{
	return Private::BattleHill::instance()->calibrateAccel();
}

short AccelX::value() const
{
	return Private::BattleHill::instance()->getAccelX();
}

short AccelY::value() const
{
	return Private::BattleHill::instance()->getAccelY();
}

short AccelZ::value() const
{
	return Private::BattleHill::instance()->getAccelZ();
}

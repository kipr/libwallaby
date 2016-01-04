/*
 * magneto.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/magneto.hpp"
#include "battlehill_p.hpp"

signed short Magneto::x()
{
	return Private::BattleHill::instance()->getMagnetoX();
}

signed short Magneto::y()
{
	return Private::BattleHill::instance()->getMagnetoY();
}

signed short Magneto::z()
{
	return Private::BattleHill::instance()->getMagnetoZ();
}

bool Magneto::calibrate()
{
	return Private::BattleHill::instance()->calibrateMagneto();
}

short MagnetoX::value() const
{
	return Private::BattleHill::instance()->getMagnetoX();
}

short MagnetoY::value() const
{
	return Private::BattleHill::instance()->getMagnetoY();
}

short MagnetoZ::value() const
{
	return Private::BattleHill::instance()->getMagnetoZ();
}

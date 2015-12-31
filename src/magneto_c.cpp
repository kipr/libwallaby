/*
 * magneto_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/magneto.h"
#include "battlehill_p.hpp"

short magneto_x()
{
	return Private::BattleHill::instance()->getMagnetoX();
}

short magneto_y()
{
	return Private::BattleHill::instance()->getMagnetoY();
}

short magneto_z()
{
	return Private::BattleHill::instance()->getMagnetoZ();
}

int magneto_calibrate()
{
	return Private::BattleHill::instance()->calibrateMagneto() ? 0 : -1;
}

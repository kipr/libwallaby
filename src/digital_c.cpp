/*
 * digital_c.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/digital.h"
#include "wallaby_p.hpp"

int digital(int port)
{
	return get_digital_value(port);
}

void set_digital_value(int port, int value)
{
	Private::BattleHill::instance()->setDigitalValue(port, value == 0 ? false : true);
}

int get_digital_value(int port)
{
	return (Private::BattleHill::instance()->getDigitalValue(port) ? 1 : 0);
}

void set_digital_output(int port, int out)
{

	Private::BattleHill::instance()->setDigitalOutput(port, out == 0 ? false : true);
}

int get_digital_output(int port)
{
	return (Private::BattleHill::instance()->getDigitalOutput(port) ? 1 : 0);
}

int get_digital_pullup(int port)
{
	// Not available on the Wallaby
	return 0;
}

void set_digital_pullup(int port, int pullup)
{
	// Not available on the Wallaby
}

/*
 * digital_c.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/digital.h"
#include "digital_p.hpp"

int digital(int port)
{
	return Private::digital_value(port, nullptr);
}

void set_digital_value(int port, int value)
{
	Private::set_digital_value(port, value == 0 ? false : true);
}

int get_digital_value(int port)
{
	return (Private::digital_value(port, nullptr) ? 1 : 0);
}

void set_digital_output(int port, int out)
{
	Private::set_digital_direction(port, out == 0 ? false : true);
}

int get_digital_output(int port)
{
	return (Private::digital_output(port, nullptr) ? 1 : 0);
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

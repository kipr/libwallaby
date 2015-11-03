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
	return !get_digital_value(port); // TODO: why is it !(...) ???
}

void set_digital_value(int port, int value)
{
	Private::Digital::instance()->setValue(port, value);
}

int get_digital_value(int port)
{
	return Private::Digital::instance()->value(port);
}

void set_digital_output(int port, int out)
{
	Private::Digital::instance()->setDirection(port, out ? Private::Digital::Out : Private::Digital::In);
}

int get_digital_output(int port)
{
	return Private::Digital::instance()->direction(port) == Private::Digital::Out;
}

int get_digital_pullup(int port)
{
	return Private::Digital::instance()->pullup(port);
}

void set_digital_pullup(int port, int pullup)
{
	Private::Digital::instance()->setPullup(port, pullup);
}

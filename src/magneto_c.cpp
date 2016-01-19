/*
 * magneto_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/magneto.h"
#include "magneto_p.hpp"

short magneto_x()
{
	return Private::magneto_x(nullptr);
}

short magneto_y()
{
	return Private::magneto_y(nullptr);
}

short magneto_z()
{
	return Private::magneto_z(nullptr);
}

int magneto_calibrate()
{
	return Private::magneto_calibrate();
}

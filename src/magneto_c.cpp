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
	return Private::Magneto::instance()->xMagneto();
}

short magneto_y()
{
	return Private::Magneto::instance()->yMagneto();
}

short magneto_z()
{
	return Private::Magneto::instance()->zMagneto();
}

int magneto_calibrate()
{
	return Private::Magneto::instance()->calibrate() ? 0 : -1;
}

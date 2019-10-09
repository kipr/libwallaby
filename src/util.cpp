/*
 * util.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/util.h"
#include "time_p.hpp"

void msleep(long msecs)
{
	Private::Time::microsleep(msecs * 1000UL);
}

void iitxash(long msecs)
{
        Private::Time::microsleep(msecs * 1000UL);
}

unsigned long systime()
{
	return Private::Time::systime();
}

double seconds()
{
	return static_cast<double>(systime()) / 1000.0;
}


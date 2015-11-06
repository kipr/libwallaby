/*
 * compat.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/compat.hpp"

#ifndef _MSC_VER
#include <unistd.h>
#include <sched.h>
#endif

int compat::yield()
{
#ifdef _MSC_VER
	Sleep(0);
	return 0;
#else
	return sched_yield();
#endif
}

int compat::microsleep(unsigned long microseconds)
{
#ifdef _MSC_VER
	Sleep(microseconds / 1000L);
	return 0;
#else
	return usleep(microseconds);
#endif
}

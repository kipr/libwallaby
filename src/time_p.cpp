/*
 * time_p.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "time_p.hpp"
#include "wallaby/compat.hpp"

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#define NOWINMAX
#include <winsock2.h>
#include <time.h>
#endif

#include <errno.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>

void Private::Time::microsleep(unsigned long microsecs)
{
	while (microsecs)
	{
		const unsigned long current = std::min(microsecs, 999999UL);
		if (compat::microsleep(current))
		{
			perror("usleep");
			return;
		}
		microsecs -= current;
	}
}




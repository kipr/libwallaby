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

unsigned long Private::Time::systime()
{
#ifdef _MSC_VER
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((ULONGLONG)ft.dwLowDateTime + ((ULONGLONG)ft.dwHighDateTime << 32)) / 10000UL;
#else
	timeval t;
	gettimeofday(&t, 0);
	return ((unsigned long)t.tv_sec) * 1000L + t.tv_usec / 1000L;
#endif
}

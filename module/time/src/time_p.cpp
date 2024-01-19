#include "time_p.hpp"
#include "kipr/compat/compat.hpp"

#include <unistd.h>
#include <sys/time.h>

#include <errno.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>

#include <unistd.h>
#include <sched.h>

void kipr::time::microsleep(unsigned long microsecs)
{
	while (microsecs)
	{
		const unsigned long current = std::min(microsecs, 999999UL);
		usleep(current);
		microsecs -= current;
	}
}

unsigned long kipr::time::systime()
{
	timeval t;
	gettimeofday(&t, 0);
	return ((unsigned long)t.tv_sec) * 1000L + t.tv_usec / 1000L;
}

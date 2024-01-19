#include "kipr/time/time.h"
#include "time_p.hpp"

using namespace kipr;
using namespace kipr::time;

void msleep(long msecs)
{
  microsleep(msecs * 1000UL);
}

unsigned long systime()
{
	return time::systime();
}

double seconds()
{
	return static_cast<double>(time::systime()) / 1000.0;
}

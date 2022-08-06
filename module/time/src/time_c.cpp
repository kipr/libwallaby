#include "kipr/time/time.h"
#include "time_p.hpp"

using namespace kipr;
using namespace kipr::time;

void msleep(long msecs)
{
  microsleep(msecs * 1000UL);
}
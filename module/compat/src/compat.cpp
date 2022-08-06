#include "kipr/compat/compat.hpp"

#ifndef _MSC_VER
#include <unistd.h>
#include <sched.h>
#endif

int kipr::compat::yield()
{
#ifdef _MSC_VER
  Sleep(0);
  return 0;
#else
  return sched_yield();
#endif
}

int kipr::compat::microsleep(unsigned long microseconds)
{
#ifdef _MSC_VER
  Sleep(microseconds / 1000L);
  return 0;
#else
  return usleep(microseconds);
#endif
}

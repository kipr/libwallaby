#include "kipr/compat/compat.hpp"

#include <unistd.h>
#include <sched.h>

int kipr::compat::yield()
{
  return sched_yield();
}

int kipr::compat::microsleep(unsigned long microseconds)
{
  return usleep(microseconds);
}

#include "kipr/digital/digital.h"
#include "digital_p.hpp"

using namespace kipr;
using namespace kipr::digital;

int digital(int port)
{
  return digital_value(port);
}

void set_digital_value(int port, int value)
{
  digital::set_digital_value(port, value == 0 ? false : true);
}

int get_digital_value(int port)
{
  return (digital_value(port) ? 1 : 0);
}

void set_digital_output(int port, int out)
{
  set_digital_direction(port, out == 0 ? false : true);
}

int get_digital_output(int port)
{
  return (digital_output(port) ? 1 : 0);
}

int get_digital_pullup(int port)
{
  // Not available on the Wallaby
  return 0;
}

void set_digital_pullup(int port, int pullup)
{
  // Not available on the Wallaby
}

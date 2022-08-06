#include "kipr/battery/battery.hpp"

#include "battery_p.hpp"

using namespace kipr;
using namespace kipr::battery;

bool Battery::isCharging()
{
  return false; // this board doesn't support charging
}

float Battery::powerLevel(unsigned int battery_type)
{
  return battery_power_level(static_cast<Type>(battery_type));
}

unsigned short Battery::rawPowerADC()
{
  return battery_raw_reading();
}

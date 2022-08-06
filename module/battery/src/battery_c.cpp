#include "kipr/battery/battery.h"
#include "kipr/battery/battery.hpp"
#include "battery_p.hpp"

using namespace kipr;
using namespace kipr::battery;

int battery_charging()
{
  return Battery::isCharging();
}

float power_level(unsigned int battery_type)
{
  return Battery::powerLevel(battery_type);
}

float power_level()
{
  return Battery::powerLevel();
}

float power_level_nimh()
{
  return Battery::powerLevel(static_cast<unsigned int>(Type::NiMH));
}

float power_level_lipo()
{
  return Battery::powerLevel(static_cast<unsigned int>(Type::LiPo));
}

float power_level_life()
{
  return Battery::powerLevel(static_cast<unsigned int>(Type::LiFe));
}

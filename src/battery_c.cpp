#include "wallaby/battery.h"
#include "wallaby/battery.hpp"

int battery_charging()
{
	return Battery::isCharging();
}

float power_level()
{
	return Battery::powerLevel();
}

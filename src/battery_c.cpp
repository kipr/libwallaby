#include "wallaby/battery.h"
#include "wallaby/battery.hpp"
#include "battery_p.hpp"

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
	return Battery::powerLevel(Private::Battery::NiMH);
}

float power_level_lipo()
{
	return Battery::powerLevel(Private::Battery::LiPo);
}

float power_level_life()
{
	return Battery::powerLevel(Private::Battery::LiFe);
}

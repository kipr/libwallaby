#ifndef _WIN32

#include "ardrone_emergency_stop_p.hpp"
#include "wallaby/ardrone.hpp"
#include "wallaby/button.hpp"
#include "wallaby/util.h"

using namespace Private;

ARDroneEmergencyStop::ARDroneEmergencyStop(ARDrone *drone)
	: m_drone(drone),
	m_stop(false),
	m_enabled(false)
{
}

void ARDroneEmergencyStop::run()
{
	m_stop = false;
	while(!m_stop) {
		msleep(200);
		if(!m_enabled || Button::Side.isNotPressed()) continue;
		
		m_drone->disconnect();
		_Exit(0);
	}
}

void ARDroneEmergencyStop::stop()
{
	m_stop = true;
}

void ARDroneEmergencyStop::setEnabled(const bool enabled)
{
	m_enabled = enabled;
}

bool ARDroneEmergencyStop::isEnabled() const
{
	return m_enabled;
}

#endif

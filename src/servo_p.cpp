/*
 * servo_p.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "servo_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;


Servo::Servo()
{

}


Servo::~Servo()
{

}


void Servo::setEnabled(int port, bool enabled)
{
	if (port > 3) return;

	unsigned short allStop = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_SRV_ALLSTOP);

	const unsigned short bit = 1 << (port + 4);
	const bool currentlyEnabled = allStop & bit;

	if (enabled == currentlyEnabled) return;

	if (enabled)
	{
		allStop |= bit;
	}
	else
	{
		allStop &= ~bit;
	}

	Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_SRV_ALLSTOP, allStop);
}


bool Servo::isEnabled(int port)
{
	if (port > 3) return false;

	unsigned short allStop = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_SRV_ALLSTOP);

	const unsigned short bit = 1 << (port + 4);
	const bool currentlyEnabled = allStop & bit;

	return currentlyEnabled;
}


bool Servo::setPosition(int port, unsigned short position)
{
	if (port > 3) return false;
	// TODO: manipulate/scale position?

	unsigned char address = REG_RW_SERVO_0_H + 2 * port;
	Private::Wallaby::instance()->writeRegister16b(address, position);
	return true;
}


unsigned short Servo::position(int port) const
{
	if (port > 3) return 0xFFFF;

	unsigned char address = REG_RW_SERVO_0_H + 2 * port;
	const unsigned short val = Private::Wallaby::instance()->readRegister16b(address);

	// TODO: manipulate position value?
	return val;
}


Servo * Servo::instance()
{
	static Servo instance;
	return & instance;
}

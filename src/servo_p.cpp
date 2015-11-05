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
	if (position > 1023) position = 1023;

	// map a 10 bit (0-1023) position to  1500 +/- (0 to 90) degrees*10
	// or    1500 +/- 900  or  [600, 2400]
	unsigned short val =  1500.0 + 900.0 * ((double)position / 1023.0) - (1023.0 / 2.0);

	unsigned char address = REG_RW_SERVO_0_H + 2 * port;
	Private::Wallaby::instance()->writeRegister16b(address, position);
	return true;
}


unsigned short Servo::position(int port) const
{
	if (port > 3) return 0xFFFF;

	unsigned char address = REG_RW_SERVO_0_H + 2 * port;
	const unsigned short position = Private::Wallaby::instance()->readRegister16b(address);

	double degrees = ((double)position - 1500.0) / 10.0; // [-90, 90]
	double dval = (degrees + 90.0)  * 1023.0 / 180.0; // [0, 1023]

	if (dval < 0.0) dval = 0.0;
	if (dval > 1023.0) dval = 1023.01;
	unsigned short val = static_cast<unsigned short>(dval);

	return val;
}


Servo * Servo::instance()
{
	static Servo instance;
	return & instance;
}

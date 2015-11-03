/*
 * analog_p.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#include "analog_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"


using namespace Private;

Analog::~Analog()
{

}

void Analog::setPullup(unsigned char port, bool pullup)
{
	// TODO: bounds checks
	unsigned char pullups = Private::Wallaby::instance()->readRegister8b(REG_RW_ADC_PE);

	if (pullup)
	{
		pullups |= (1 << port);
	}
	else
	{
		pullups &= ~(1 << port);
	}

	Private::Wallaby::instance()->writeRegister8b(REG_RW_ADC_PE, pullups);
}

bool Analog::pullup(unsigned char port) const
{
	// TODO: bounds checking
	unsigned char pullups = Private::Wallaby::instance()->readRegister8b(REG_RW_ADC_PE);

	return pullups & (1 << port);
}

unsigned short Analog::value(unsigned char port) const
{
	// TODO: bounds checking
	const unsigned char address = REG_RW_ADC_0_H + 2 * port;
	const unsigned short val = Private::Wallaby::instance()->readRegister16b(address);
	return val;
}

Analog * Analog::instance()
{
	static Analog instance;
	return &instance;
}

Analog::Analog()
{

}

Analog::Analog(const Analog & rhs)
{

}

Analog & Analog::operator=(const Analog & rhs)
{
	return *this;
}



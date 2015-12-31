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



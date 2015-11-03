/*
 * digital_p.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "digital_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

Digital::~Digital()
{

}

bool Digital::value(unsigned char port) const
{
	// TODO which port is 0?
	// TODO: boundary checking

	unsigned short dig_ins_val =  Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_IN_H);

	// TODO: what if this isn't a digital in
	bool ret = dig_ins_val & (1 << port);
	return ret;
}


bool Digital::setValue(unsigned char port, bool value)
{
	// TODO boundary checks

	unsigned short out = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OUT_H);

	if (value)
	{
		out |= (1 << port);
	}
	else
	{
		out &= ~(1 << port);
	}

	Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OUT_H, out);

	return true; //TODO: return based on success
}




const Digital::Direction Digital::direction(unsigned char port) const
{
	// TODO: bounds check, if not a good port return Digital::Unknown

	unsigned short outputs = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OE_H);
	Digital::Direction ret = outputs & (1 << port) ? Digital::Out : Digital::In;
	return ret;
}


bool Digital::setDirection(unsigned char port, const Digital::Direction & direction)
{
	// TODO: bounds check, if not a good port return Digital::Unknown

	if (direction == Digital::Unknown) return false;

	unsigned short outputs = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OE_H);

	if (direction == Digital::Out)
	{
		outputs |= (1 << port);
	}
	else if (direction == Digital::In)
	{
		outputs &= ~(1 << port);
	}

	Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OE_H, outputs);

	return true; // TODO: based on success;
}


bool Digital::pullup(unsigned char port) const
{
	// TODO: bounds checking (return false if out of range)

	unsigned short pullups = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_PE_H);

	bool ret = pullups & (1 << port);
	return ret;
}

bool Digital::setPullup(unsigned char port, bool pullup)
{
	// TODO: bounds checking (return false if out of range)
	unsigned short pullups = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_PE_H);

	if (pullup)
	{
		pullups |= (1 << port);
	}
	else
	{
		pullups &= ~(1 << port);
	}

	Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_PE_H, pullups);

	return true; // TODO: based on success;
}

Digital * Digital::instance()
{
	static Digital instance;
	return &instance;
}

void Digital::wiggle() const
{
	return;// FIXME implement
}

Digital::Digital()
{

}

Digital::Digital(const Digital & rhs)
{

}

Digital & Digital::operator=(const Digital & rhs)
{
	return *this;
}

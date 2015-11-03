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

	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that
	// TODO: or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_IN_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_IN_L, low_byte);

	unsigned short dig_ins_val = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	// TODO: what if this isn't a digital in
	bool ret = dig_ins_val & (1 << port);
	return ret;
}


bool Digital::setValue(unsigned char port, bool value)
{
	// TODO boundary checks
	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that
	// TODO: or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OUT_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OUT_L, low_byte);

	unsigned short out = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	if (value)
	{
		out |= (1 << port);
	}
	else
	{
		out &= ~(1 << port);
	}

	high_byte = static_cast<unsigned char>((out & 0xFF00) >> 8);
	low_byte = static_cast<unsigned char>(out & 0x00FF);

	bool success = Private::Wallaby::instance()->writeRegister(REG_RW_DIG_OUT_H, high_byte)
			& Private::Wallaby::instance()->writeRegister(REG_RW_DIG_OUT_L, low_byte);

	return success;
}




const Digital::Direction Digital::direction(unsigned char port) const
{
	// TODO: bounds check, if not a good port return Digital::Unknown

	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that
	// TODO: or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OE_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OE_L, low_byte);

	unsigned short outputs = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);
	Digital::Direction ret = outputs & (1 << port) ? Digital::Out : Digital::In;
	return ret;
}


bool Digital::setDirection(unsigned char port, const Digital::Direction & direction)
{
	// TODO: bounds check, if not a good port return Digital::Unknown

	if (direction == Digital::Unknown) return false;

	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that, or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OE_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_OE_L, low_byte);

	unsigned short outputs = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	if (direction == Digital::Out)
	{
		outputs |= (1 << port);
	}
	else if (direction == Digital::In)
	{
		outputs &= ~(1 << port);
	}

	high_byte = static_cast<unsigned char>((outputs & 0xFF00) >> 8);
	low_byte = static_cast<unsigned char>(outputs & 0x00FF);


	bool success = Private::Wallaby::instance()->writeRegister(REG_RW_DIG_OE_H, high_byte)
			& Private::Wallaby::instance()->writeRegister(REG_RW_DIG_OE_L, low_byte);

	return success;
}


bool Digital::pullup(unsigned char port) const
{
	// TODO: bounds checking (return false if out of range)
	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that, or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_PE_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_PE_L, low_byte);

	unsigned short pullups = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	bool ret = pullups & (1 << port);
	return ret;
}

bool Digital::setPullup(unsigned char port, bool pullup)
{
	// TODO: bounds checking (return false if out of range)
	unsigned char high_byte, low_byte;

	// TODO: read both values at the same time, once we support that, or just the byte we need
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_PE_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_DIG_PE_L, low_byte);

	unsigned short pullups = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	if (pullup)
	{
		pullups |= (1 << port);
	}
	else
	{
		pullups &= ~(1 << port);
	}

	high_byte = static_cast<unsigned char>((pullups & 0xFF00) >> 8);
	low_byte = static_cast<unsigned char>(pullups & 0x00FF);


	bool success = Private::Wallaby::instance()->writeRegister(REG_RW_DIG_PE_H, high_byte)
			& Private::Wallaby::instance()->writeRegister(REG_RW_DIG_PE_L, low_byte);

	return success;
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

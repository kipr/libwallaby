/*
 * button_p.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: kipr
 */


#include "button_p.hpp"
#include "wallaby_p.hpp"
#include "digital_p.hpp"
#include "wallaby_regs_p.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace Private;


void Private::Button::setPressed(const ::Button::Type::Id &id, bool pressed)
{
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return;

	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);

	if(pressed) states |= (1 << offset);
	else states &= ~(1 << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_BUTTONS, states);
}


bool Private::Button::isPressed(const ::Button::Type::Id &id) const
{
	// Physical buttons:
	if (id == ::Button::Type::Left)
	{
		// TODO read from gpio
	}

	if (id == ::Button::Type::Right)
	{
		return Private::digital_value(13, nullptr);
	}

	// Virtual buttons:
	const unsigned char offset = buttonOffset(id);
	if (offset > 5) return false;

	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	return (states & 1 << offset) ? true : false;
}


void Private::Button::setExtraShown(const bool& shown)
{
	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	if (shown)
	{
		states |= 0b10000000;
	}
	else
	{
		states &= 0b01111111;
	}
	Private::Wallaby::instance()->writeRegister8b(REG_RW_BUTTONS, states);
}


bool Private::Button::isExtraShown() const
{
	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	return (states & 0b10000000) ? true : false;
}


Private::Button *Private::Button::instance()
{
	static Button s_button;
	return &s_button;
}


unsigned char Private::Button::buttonOffset(const ::Button::Type::Id &id) const
{
	return (unsigned char)id;
}


Private::Button::Button()
{
}


Private::Button::Button(const Button& rhs)
{
}



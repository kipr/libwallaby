/*
 * button_p.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#include "button_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace Private;

void Private::Button::setText(const ::Button::Type::Id & id, const char * text)
{
	//FIXME: implement
}

bool Private::Button::isTextDirty(const ::Button::Type::Id & id) const
{
	//FIXME: implement
	return true;
}

const char * Private::Button::text(const ::Button::Type::Id & id) const
{
	//FIXME: implement
	return "";
}

void Private::Button::setPressed(const ::Button::Type::Id & id, bool pressed)
{
	//FIXME: implement
}

bool Private::Button::isPressed(const ::Button::Type::Id & id) const
{
	// FIXME: implement
	return true;
}

void Private::Button::resetButtons()
{
	strncpy(m_text[0], "A", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[1], "B", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[2], "C", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[3], "X", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[4], "Y", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[5], "Z", MAX_BUTTON_TEXT_SIZE);
}

void Private::Button::setExtraShown(bool shown)
{
	//FIXME: implement
}

bool Private::Button::isExtraShown() const
{
	//FIXME: implement
	return true;
}

Private::Button * Private::Button::instance()
{
	static Private::Button instance;
	return &instance;
}


unsigned char Private::Button::buttonOffset(const ::Button::Type::Id & id) const
{
	return (unsigned char) id;
}

bool Private::Button::buttonRegs(unsigned short & start, unsigned short & end, ::Button::Type::Id id) const
{
	// TODO:
	return false;
}

Private::Button::Button()
{
	resetButtons();
}

Private::Button::Button(const Button & rhs)
{

}

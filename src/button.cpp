/*
 * button.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/button.hpp"
#include "button_p.hpp"
#include "wallaby/compat.hpp"

#include <cstring>

IdButton Button::A(Button::Type::A, "A");
IdButton Button::B(Button::Type::B, "B");
IdButton Button::C(Button::Type::C, "C");

IdButton Button::X(Button::Type::X, "X");
IdButton Button::Y(Button::Type::Y, "Y");
IdButton Button::Z(Button::Type::Z, "Z");


IdButton Button::Side(Button::Type::Side, "");

IdButton * const Button::all[7] = {
		&Button::A, &Button::B, &Button::C,
		&Button::X, &Button::Y, &Button::Z,
		&Button::Side
};

AbstractButton::~AbstractButton()
{

}

void AbstractButton::waitUntilReleased() const
{
	while (isPressed()) compat::yield();
}

void AbstractButton::waitUntilPressed() const
{
	while (isNotPressed()) compat::yield();
}

void AbstractButton::waitUntilClicked() const
{
	waitUntilPressed();
	waitUntilReleased();
}

AbstractTextButton::~AbstractTextButton()
{

}

IdButton::~IdButton()
{
	if (m_defaultText) delete[] m_defaultText;
}

void IdButton::setText(const char * text)
{
	Private::Button::instance()->text(m_id);
}

const char * IdButton::text() const
{
	return Private::Button::instance()->text(m_id);
}

void IdButton::setPressed(bool pressed)
{
	Private::Button::instance()->setPressed(m_id, pressed);
}

bool IdButton::value() const
{
	return Private::Button::instance()->isPressed(m_id);
}

void IdButton::resetText()
{
	setText(m_defaultText);
}

void ExtraButtons::show()
{
	setShown(true);
}

void ExtraButtons::hide()
{
	setShown(false);
}

void ExtraButtons::setShown(bool shown)
{
	Private::Button::instance()->setExtraShown(shown);
}

bool ExtraButtons::isShown()
{
	return Private::Button::instance()->isExtraShown();
}

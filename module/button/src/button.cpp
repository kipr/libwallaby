/*
 * button.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#include "button_p.hpp"
#include "wallaby/button.hpp"
#include "wallaby_p.hpp"
#include "wallaby/compat.hpp"


#include <cstring>

IdButton Button::A(Button::Type::A, "A");
IdButton Button::B(Button::Type::B, "B");
IdButton Button::C(Button::Type::C, "C");

IdButton Button::X(Button::Type::X, "X");
IdButton Button::Y(Button::Type::Y, "Y");
IdButton Button::Z(Button::Type::Z, "Z");

IdButton Button::Left(Button::Type::Left, "");
IdButton Button::Right(Button::Type::Right, "");

IdButton * const Button::all[8] = {
		&Button::A, &Button::B, &Button::C,
		&Button::X, &Button::Y, &Button::Z,
                &Button::Left, &Button::Right
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

IdButton::IdButton(const Button::Type::Id & id, const char * defaultText)
:m_id(id)
{
	size_t len = strlen(defaultText);
	m_defaultText = new char[len + 1];
	memcpy(m_defaultText, defaultText, len);
	m_defaultText[len] = 0;
}

IdButton::~IdButton()
{
	if (m_defaultText) delete[] m_defaultText;
}

void IdButton::setText(const char * text)
{
	// deprecated
}

const char * IdButton::text() const
{
	return m_defaultText; // deprecated
}

bool IdButton::isTextDirty() const
{
	return false; // deprecated
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

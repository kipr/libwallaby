#include "button_p.hpp"
#include "kipr/button/button.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/compat/compat.hpp"

#include <cstring>

using namespace kipr;
using namespace kipr::button;

IdButton kipr::button::A(Id::A, "A");
IdButton kipr::button::B(Id::B, "B");
IdButton kipr::button::C(Id::C, "C");

IdButton kipr::button::X(Id::X, "X");
IdButton kipr::button::Y(Id::Y, "Y");
IdButton kipr::button::Z(Id::Z, "Z");

IdButton kipr::button::Left(Id::Left, "");
IdButton kipr::button::Right(Id::Right, "");

IdButton * const kipr::button::all[8] = {
    &A, &B, &C,
    &X, &Y, &Z,
    &Left, &Right
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

IdButton::IdButton(const Id &id, const char * defaultText)
  : m_id(id)
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
  Button::instance()->setPressed(m_id, pressed);
}

bool IdButton::value() const
{
  return Button::instance()->isPressed(m_id);
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
  Button::instance()->setExtraShown(shown);
}

bool ExtraButtons::isShown()
{
  return Button::instance()->isExtraShown();
}

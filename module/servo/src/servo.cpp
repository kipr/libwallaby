#include "kipr/servo/servo.hpp"
#include "servo_p.hpp"

using namespace kipr;
using namespace kipr::servo;

Servo::Servo(int port)
    : m_port(port)
{
}

void Servo::setPosition(Servo::ticks_t position)
{
  set_servo_position(m_port, position);
}

Servo::ticks_t Servo::position() const
{
  return get_servo_position(m_port);
}

void Servo::disable()
{
  setEnabled(false);
}

void Servo::enable()
{
  setEnabled(true);
}

void Servo::setEnabled(bool enabled)
{
  set_servo_enabled(m_port, enabled);
}

bool Servo::isEnabled() const
{
  return get_servo_enabled(m_port);
}

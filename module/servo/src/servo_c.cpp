#include "kipr/servo/servo.h"
#include "servo_p.hpp"

void enable_servo(int port)
{
  set_servo_enabled(port, 1);
}

void disable_servo(int port)
{
  set_servo_enabled(port, 0);
}

void enable_servos()
{
  for (int i = 0; i < 4; ++i)
    enable_servo(i);
}

void disable_servos()
{
  for (int i = 0; i < 4; ++i)
    disable_servo(i);
}

void set_servo_enabled(int port, int enabled)
{
  kipr::servo::set_servo_enabled(port, enabled ? true : false);
}

int get_servo_enabled(int port)
{
  return (kipr::servo::get_servo_enabled(port) ? 1 : 0);
}

int get_servo_position(int port)
{
  return kipr::servo::get_servo_position(port);
}

void set_servo_position(int port, int position)
{
  kipr::servo::set_servo_position(port, position);
}

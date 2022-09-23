#include "kipr/motor/motor.h"
#include "kipr/time/time.h"
#include "motor_p.hpp"
#include "kipr/motor/command.hpp"
#include "kipr/core/platform.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace kipr;
using namespace kipr::motor;

using kipr::core::Platform;
using kipr::core::Memory;

int get_motor_position_counter(int motor)
{
  return get_motor_bemf(motor);
}

int gmpc(int motor)
{
  return get_motor_position_counter(motor);
}

void clear_motor_position_counter(int motor)
{
  motor::clear_motor_bemf(motor);
}

void cmpc(int motor)
{
  clear_motor_position_counter(motor);
}

int move_at_velocity(int motor, int velocity)
{
  set_motor_mode(motor, static_cast<unsigned char>(ControlMode::Speed));
  set_motor_goal_velocity(motor, velocity);
  return 0;
}

int mav(int motor, int velocity)
{
  return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
  // FIXME: handle velocity scaling?
  const int sign = motor::get_motor_bemf(motor) > goal_pos ? -1 : 1;
  const short velocity = std::abs(speed) * sign;

  // TODO: combine into one call
  set_motor_mode(motor, static_cast<unsigned char>(ControlMode::SpeedPosition));
  set_motor_goal_position(motor, goal_pos);
  set_motor_goal_velocity(motor, velocity);

  return 0;
}

int mtp(int motor, int speed, int goal_pos)
{
  return move_to_position(motor, speed, goal_pos);
}

int move_relative_position(int motor, int speed, int delta_pos)
{
  if (motor < 0 || motor > 3) return -1;
  move_to_position(motor, speed, get_motor_bemf(motor) + delta_pos);
  return 0;
}

int mrp(int motor, int speed, int delta_pos)
{
  return move_relative_position(motor, speed, delta_pos);
}

void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd)
{
  set_motor_pid_gains(motor, p, i, d, pd, id, dd);
}

void get_pid_gains(int motor, short * p, short * i, short * d, short * pd, short * id, short * dd)
{
  get_motor_pid_gains(motor, *p, *i, *d, *pd, *id, *dd);
}

int freeze(int motor)
{
  set_motor_pwm(motor, 100);
  set_motor_direction(motor, static_cast<unsigned char>(Direction::ActiveStop));
  return 0;
}

int get_motor_done(int motor)
{
  if (motor < 0 || motor > 3) return -1;
  msleep(2); // to make sure PID was run.. TODO: remove?
  return (get_motor_pid_active(motor) ? 0 : 1);
}

void block_motor_done(int motor)
{
  while (!get_motor_done(motor)) msleep(2);
}

void bmd(int motor)
{
  block_motor_done(motor);
}

int setpwm(int motor, int pwm)
{
  set_motor_pwm(motor, pwm);
  return -1;
}

int getpwm(int motor)
{
  return -1;
}

void fd(int motor)
{
  ::motor(motor, 100);
}

void bk(int motor)
{
  ::motor(motor, -100);
}

void motor(int motor, int percent)
{
  mav(motor, percent*15); // 100 percent = 1500 ticks/sec
}

void baasbennaguui(int motor, int percent)
{
  mav(motor, percent*15); // 100 percent = 1500 ticks/sec
}

void motor_power(int motor, int percent)
{
  set_motor_mode(motor, static_cast<unsigned char>(ControlMode::Inactive));
  set_motor_pwm(motor, std::abs(percent));

  if (percent > 0)
  {
    set_motor_direction(motor, static_cast<unsigned char>(Direction::Forward));
  }
  else if (percent < 0)
  {
    set_motor_direction(motor, static_cast<unsigned char>(Direction::Reverse));
  }
  else
  {
    set_motor_direction(motor, static_cast<unsigned char>(Direction::PassiveStop));
  }
}

void off(int motor)
{
  stop_motor(motor);
}

void alloff()
{
  for (unsigned int i = 0; i < 4; ++i) off(i);
}

void ao()
{
  alloff();
}

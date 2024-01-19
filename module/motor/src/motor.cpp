/*
 * motors.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "kipr/time/time.h"
#include "kipr/motor/motor.hpp"
#include "motor_p.hpp"

#include <cstdlib>
#include <math.h>

using namespace kipr;
using namespace kipr::motor;

Motor::Motor(int port)
: m_port(port)
{

}

void Motor::clearPositionCounter()
{
  motor::clear_motor_bemf(m_port);
}

void Motor::moveAtVelocity(short velocity)
{
  motor::set_motor_mode(m_port, static_cast<unsigned char>(ControlMode::Speed));
  motor::set_motor_goal_velocity(m_port, velocity);
}

void Motor::moveToPosition(short speed, int goalPos)
{
  // FIXME: handle velocity scaling?
  const int sign = motor::get_motor_bemf(m_port) > goalPos ? -1 : 1;
  const short velocity = std::abs(speed) * sign;

  motor::set_motor_mode(m_port, static_cast<unsigned char>(ControlMode::SpeedPosition));
  motor::set_motor_goal_position(m_port, goalPos);
  motor::set_motor_goal_velocity(m_port, velocity);
}

void Motor::moveRelativePosition(short speed, int deltaPos)
{
  moveToPosition(speed, motor::get_motor_bemf(m_port) + deltaPos);
}

void Motor::setPidGains(short p, short i, short d, short pd, short id, short dd)
{
  motor::set_motor_pid_gains(m_port, p, i, d, pd, id, dd);
}

void Motor::pidGains(short & p, short & i, short & d, short & pd, short & id, short & dd)
{
  motor::get_motor_pid_gains(m_port, p, i, d, pd, id, dd);
}

void Motor::freeze()
{
  motor::set_motor_pwm(m_port, 100);
  motor::set_motor_direction(m_port, static_cast<unsigned char>(Direction::ActiveStop));
}

bool Motor::isMotorDone() const
{
  return !(motor::get_motor_pid_active(m_port));
}

void Motor::blockMotorDone() const
{
  msleep(5); // TO make sure PID loop ran, TODO: remove?
  while(isMotorDone()) msleep(5);
}

void Motor::forward()
{
  motor(100);
}

void Motor::backward()
{
  motor(-100);
}


void Motor::motor(int percent)
{
  moveAtVelocity(percent * 15); // 100% = 1500 ticks/sec
}


void Motor::baasbennaguui(int percent)
{
        moveAtVelocity(percent * 15); // 100% = 1500 ticks/sec
}

void Motor::motorPower(int percent)
{
  motor::set_motor_mode(m_port, static_cast<unsigned char>(ControlMode::Inactive));
  motor::set_motor_pwm(m_port, std::abs(percent));

  if (percent > 0)
  {
    motor::set_motor_direction(m_port, static_cast<unsigned char>(Direction::Forward));
  }
  else if (percent < 0)
  {
    motor::set_motor_direction(m_port, static_cast<unsigned char>(Direction::Reverse));
  }
  else
  {
    motor::set_motor_direction(m_port, static_cast<unsigned char>(Direction::PassiveStop));
  }
}

void Motor::off()
{
  motor::stop_motor(m_port);
}

int Motor::port() const
{
  return m_port;
}

BackEMF::BackEMF(int port)
: m_port(port)
{

}

int BackEMF::value() const
{
  return motor::get_motor_bemf(m_port);
}

int BackEMF::port() const
{
  return m_port;
}

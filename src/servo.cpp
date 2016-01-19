/*
 * servo.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/servo.hpp"
#include "servo_p.hpp"

Servo::Servo(int port)
: m_port(port)
{

}

void Servo::setPosition(Servo::ticks_t position)
{
	Private::set_servo_position(m_port, position);
}


Servo::ticks_t Servo::position() const
{
	return Private::get_servo_position(m_port, nullptr);
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
	Private::set_servo_enabled(m_port, enabled);
}

bool Servo::isEnabled() const
{
	return Private::get_servo_enabled(m_port, nullptr);
}

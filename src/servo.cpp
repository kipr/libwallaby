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
	Private::Servo::instance()->setPosition(m_port, position);
}


Servo::ticks_t Servo::position() const
{
	return Private::Servo::instance()->position(m_port);
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
	Private::Servo::instance()->setEnabled(m_port, enabled);
}

bool Servo::isEnabled() const
{
	return Private::Servo::instance()->isEnabled(m_port);
}

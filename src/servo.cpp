/*
 * servo.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/servo.hpp"
#include "battlehill_p.hpp"

Servo::Servo(int port)
: m_port(port)
{

}

void Servo::setPosition(Servo::ticks_t position)
{
	Private::BattleHill::instance()->setServoPosition(m_port, position);
}


Servo::ticks_t Servo::position() const
{
	return Private::BattleHill::instance()->getServoPosition(m_port);
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
	Private::BattleHill::instance()->setServoEnabled(m_port, enabled);
}

bool Servo::isEnabled() const
{
	return Private::BattleHill::instance()->getServoEnabled(m_port);
}

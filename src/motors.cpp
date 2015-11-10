/*
 * motors.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/util.h"
#include "wallaby/motors.hpp"
#include "motors_p.hpp"

#include <cstdlib>
#include <math.h>

Motor::Motor(int port)
: m_port(port)
{

}

void Motor::clearPositionCounter()
{
	Private::Motor::instance()->clearBemf(m_port);
}

void Motor::moveAtVelocity(short velocity)
{
	Private::Motor::instance()->setControlMode(m_port, Private::Motor::Speed);
	Private::Motor::instance()->setPidVelocity(m_port, velocity);
}

void Motor::moveToPosition(short speed, int goalPos)
{
	// FIXME: handle velocity scaling?
	const int sign = Private::Motor::instance()->backEMF(m_port) > goalPos ? -1 : 1;
	const short velocity = std::abs(speed) * sign;

	Private::Motor::instance()->setControlMode(m_port, Private::Motor::SpeedPosition);
	Private::Motor::instance()->setPidGoalPos(m_port, goalPos);
	Private::Motor::instance()->setPidVelocity(m_port, velocity);
}

void Motor::moveRelativePosition(short speed, int deltaPos)
{
	moveToPosition(speed, Private::Motor::instance()->backEMF(m_port) + deltaPos);
}

void Motor::setPidGains(short p, short i, short d, short pd, short id, short dd)
{
	Private::Motor::instance()->setPidGains(m_port, p, i, d, pd, id, dd);
}

void Motor::pidGains(short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	Private::Motor::instance()->pidGains(m_port, p, i, d, pd, id, dd);
}

void Motor::freeze()
{
	Private::Motor::instance()->setPwm(m_port, 100);
	Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::ActiveStop);
}

bool Motor::isMotorDone() const
{
	return !(Private::Motor::instance()->isPidActive(m_port));
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
	Private::Motor::instance()->setControlMode(m_port, Private::Motor::Inactive);
	Private::Motor::instance()->setPwm(m_port, std::abs(percent));
	if (percent > 0)
	{
		Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Forward);
	}
	else if (percent < 0)
	{
		Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Reverse);
	}
	else
	{
		Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::PassiveStop);
	}
}

void Motor::off()
{
	Private::Motor::instance()->stop(m_port);
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
	return Private::Motor::instance()->backEMF(m_port);
}

int BackEMF::port() const
{
	return m_port;
}

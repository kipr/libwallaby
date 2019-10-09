/*
 * motors.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/util.h"
#include "wallaby/motors.hpp"
#include "motor_p.hpp"

#include <cstdlib>
#include <math.h>

Motor::Motor(int port)
: m_port(port)
{

}

void Motor::clearPositionCounter()
{
	Private::clear_motor_bemf(m_port);
}

void Motor::moveAtVelocity(short velocity)
{
	Private::set_motor_mode(m_port, Private::Motor::Speed);
	Private::set_motor_goal_velocity(m_port, velocity);
}

void Motor::moveToPosition(short speed, int goalPos)
{
	// FIXME: handle velocity scaling?
	const int sign = Private::get_motor_bemf(m_port, nullptr) > goalPos ? -1 : 1;
	const short velocity = std::abs(speed) * sign;

	Private::set_motor_mode(m_port, Private::Motor::SpeedPosition);
	Private::set_motor_goal_position(m_port, goalPos);
	Private::set_motor_goal_velocity(m_port, velocity);
}

void Motor::moveRelativePosition(short speed, int deltaPos)
{
	moveToPosition(speed, Private::get_motor_bemf(m_port, nullptr) + deltaPos);
}

void Motor::setPidGains(short p, short i, short d, short pd, short id, short dd)
{
	Private::set_motor_pid_gains(m_port, p, i, d, pd, id, dd);
}

void Motor::pidGains(short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	Private::get_motor_pid_gains(m_port, p, i, d, pd, id, dd);
}

void Motor::freeze()
{
	Private::set_motor_pwm(m_port, 100);
	Private::set_motor_direction(m_port, Private::Motor::ActiveStop);
}

bool Motor::isMotorDone() const
{
	return !(Private::get_motor_pid_active(m_port, nullptr));
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
	Private::set_motor_mode(m_port, Private::Motor::Inactive);
	Private::set_motor_pwm(m_port, std::abs(percent));

	if (percent > 0)
	{
		Private::set_motor_direction(m_port, Private::Motor::Forward);
	}
	else if (percent < 0)
	{
		Private::set_motor_direction(m_port, Private::Motor::Reverse);
	}
	else
	{
		Private::set_motor_direction(m_port, Private::Motor::PassiveStop);
	}
}



void Motor::off()
{
	Private::stop_motor(m_port);
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
	return Private::get_motor_bemf(m_port, nullptr);
}

int BackEMF::port() const
{
	return m_port;
}

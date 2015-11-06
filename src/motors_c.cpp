/*
 * motors_c.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/motors.h"
#include "wallaby/util.h"
#include "motors_p.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

int get_motor_position_counter(int motor)
{
	return Private::Motor::instance()->backEMF(motor);
}

int gmpc(int motor)
{
	return get_motor_position_counter(motor);
}

void clear_motor_position_counter(int motor)
{
	Private::Motor::instance()->clearBemf(motor);
}

void cmpc(int motor)
{
	clear_motor_position_counter(motor);
}

int move_at_velocity(int motor, int velocity)
{
	Private::Motor::instance()->setControlMode(motor, Private::Motor::Speed);
	Private::Motor::instance()->setPidVelocity(motor, velocity);
	return 0;
}

int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
	// FIXME: handle velocity scaling?
	const int sign = Private::Motor::instance()->backEMF(motor) > goal_pos ? 1 : -1;
	const short velocity = std::abs(speed) * sign;

	Private::Motor::instance()->setControlMode(motor, Private::Motor::SpeedPosition);
	Private::Motor::instance()->setPidGoalPos(motor, goal_pos);
	Private::Motor::instance()->setPidVelocity(motor, velocity);
	return 0;
}

int mtp(int motor, int speed, int goal_pos)
{
	return move_to_position(motor, speed, goal_pos);
}

int move_relative_position(int motor, int speed, int delta_pos)
{
	if (motor < 0 || motor > 3) return -1;
	move_to_position(motor, speed, Private::Motor::instance()->backEMF(motor) + delta_pos);
	return 0;
}

int mrp(int motor, int speed, int delta_pos)
{
	return move_relative_position(motor, speed, delta_pos);
}

void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd)
{
	Private::Motor::instance()->setPidGains(motor, p, i, d, pd, id, dd);
}

void get_pid_gains(int motor, short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	Private::Motor::instance()->pidGains(motor, p, i, d, pd, id, dd);
}

int freeze(int motor)
{
	Private::Motor::instance()->setPwm(motor, 100);
	Private::Motor::instance()->setPwmDirection(motor, Private::Motor::ActiveStop);
	return 0;
}

int get_motor_done(int motor)
{
	if (motor < 0 || motor > 3) return -1;
	msleep(2); // to make sure PID was run.. TODO: remove?
	return Private::Motor::instance()->isPidActive(motor) ? 0 : 1;
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
	Private::Motor::instance()->setPwm(motor, pwm);
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
	Private::Motor::instance()->setControlMode(motor, Private::Motor::Inactive);
	Private::Motor::instance()->setPwm(motor, std::abs(percent));
	if (percent > 0)
	{
		Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Forward);
	}
	else if (percent < 0)
	{
		Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Reverse);
	}
	else
	{
		Private::Motor::instance()->setPwmDirection(motor, Private::Motor::PassiveStop);
	}
}

void off(int motor)
{
	Private::Motor::instance()->stop(motor);
}

void alloff()
{
	for (unsigned int i = 0; i < 4; ++i) off(i);
}

void ao()
{
	alloff();
}

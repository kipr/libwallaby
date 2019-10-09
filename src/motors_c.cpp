/*
 * motors_c.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/motors.h"
#include "wallaby/util.h"
#include "motor_p.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

int get_motor_position_counter(int motor)
{
	return Private::get_motor_bemf(motor, nullptr);
}

int gmpc(int motor)
{
	return get_motor_position_counter(motor);
}

void clear_motor_position_counter(int motor)
{
	Private::clear_motor_bemf(motor);
}

void cmpc(int motor)
{
	clear_motor_position_counter(motor);
}

int move_at_velocity(int motor, int velocity)
{
	Private::set_motor_mode(motor, Private::Motor::Speed);
	Private::set_motor_goal_velocity(motor, velocity);
	return 0;
}

int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
	// FIXME: handle velocity scaling?
	const int sign = Private::get_motor_bemf(motor, nullptr) > goal_pos ? -1 : 1;
	const short velocity = std::abs(speed) * sign;

	// TODO: combine into one call
	Private::set_motor_mode(motor, Private::Motor::SpeedPosition);
	Private::set_motor_goal_position(motor, goal_pos);
	Private::set_motor_goal_velocity(motor, velocity);

	return 0;
}

int mtp(int motor, int speed, int goal_pos)
{
	return move_to_position(motor, speed, goal_pos);
}

int move_relative_position(int motor, int speed, int delta_pos)
{
	if (motor < 0 || motor > 3) return -1;
	move_to_position(motor, speed, Private::get_motor_bemf(motor, nullptr) + delta_pos);
	return 0;
}

int mrp(int motor, int speed, int delta_pos)
{
	return move_relative_position(motor, speed, delta_pos);
}

void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd)
{
	Private::set_motor_pid_gains(motor, p, i, d, pd, id, dd);
}

void get_pid_gains(int motor, short * p, short * i, short * d, short * pd, short * id, short * dd)
{
	Private::get_motor_pid_gains(motor, *p, *i, *d, *pd, *id, *dd);
}

int freeze(int motor)
{
	Private::set_motor_pwm(motor, 100);
	Private::set_motor_direction(motor, Private::Motor::ActiveStop);
	return 0;
}

int get_motor_done(int motor)
{
	if (motor < 0 || motor > 3) return -1;
	msleep(2); // to make sure PID was run.. TODO: remove?
	return (Private::get_motor_pid_active(motor, nullptr) ? 0 : 1);
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
	Private::set_motor_pwm(motor, pwm);
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
	Private::set_motor_mode(motor, Private::Motor::Inactive);
	Private::set_motor_pwm(motor, std::abs(percent));

	if (percent > 0)
	{
		Private::set_motor_direction(motor, Private::Motor::Forward);
	}
	else if (percent < 0)
	{
		Private::set_motor_direction(motor, Private::Motor::Reverse);
	}
	else
	{
		Private::set_motor_direction(motor, Private::Motor::PassiveStop);
	}
}

void off(int motor)
{
	Private::stop_motor(motor);
}

void alloff()
{
	for (unsigned int i = 0; i < 4; ++i) off(i);
}

void ao()
{
	alloff();
}

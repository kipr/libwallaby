/*
 * motors_c.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/motors.h"
#include "wallaby/util.h"
#include "battlehill_p.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

int get_motor_position_counter(int motor)
{
	return Private::BattleHill::instance()->backEMF(motor);
}

int gmpc(int motor)
{
	return get_motor_position_counter(motor);
}

void clear_motor_position_counter(int motor)
{
	Private::BattleHill::instance()->clearBemf(motor);
}

void cmpc(int motor)
{
	clear_motor_position_counter(motor);
}

int move_at_velocity(int motor, int velocity)
{
	Private::BattleHill::instance()->setControlMode(motor, Private::BattleHill::Speed);
	Private::BattleHill::instance()->setPidVelocity(motor, velocity);
	return 0;
}

int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
	// FIXME: handle velocity scaling?
	const int sign = Private::BattleHill::instance()->backEMF(motor) > goal_pos ? -1 : 1;
	const short velocity = std::abs(speed) * sign;

	Private::BattleHill::instance()->setControlMode(motor, Private::BattleHill::SpeedPosition);
	Private::BattleHill::instance()->setPidGoalPos(motor, goal_pos);
	Private::BattleHill::instance()->setPidVelocity(motor, velocity);
	return 0;
}

int mtp(int motor, int speed, int goal_pos)
{
	return move_to_position(motor, speed, goal_pos);
}

int move_relative_position(int motor, int speed, int delta_pos)
{
	if (motor < 0 || motor > 3) return -1;
	move_to_position(motor, speed, Private::BattleHill::instance()->backEMF(motor) + delta_pos);
	return 0;
}

int mrp(int motor, int speed, int delta_pos)
{
	return move_relative_position(motor, speed, delta_pos);
}

void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd)
{
	Private::BattleHill::instance()->setPidGains(motor, p, i, d, pd, id, dd);
}

void get_pid_gains(int motor, short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	Private::BattleHill::instance()->pidGains(motor, p, i, d, pd, id, dd);
}

int freeze(int motor)
{
	Private::BattleHill::instance()->setPwm(motor, 100);
	Private::BattleHill::instance()->setPwmDirection(motor, Private::BattleHill::ActiveStop);
	return 0;
}

int get_motor_done(int motor)
{
	if (motor < 0 || motor > 3) return -1;
	msleep(2); // to make sure PID was run.. TODO: remove?
	return Private::BattleHill::instance()->isPidActive(motor) ? 0 : 1;
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
	Private::BattleHill::instance()->setPwm(motor, pwm);
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
	Private::BattleHill::instance()->setControlMode(motor, Private::BattleHill::Inactive);
	Private::BattleHill::instance()->setPwm(motor, std::abs(percent));
	if (percent > 0)
	{
		Private::BattleHill::instance()->setPwmDirection(motor, Private::BattleHill::Forward);
	}
	else if (percent < 0)
	{
		Private::BattleHill::instance()->setPwmDirection(motor, Private::BattleHill::Reverse);
	}
	else
	{
		Private::BattleHill::instance()->setPwmDirection(motor, Private::BattleHill::PassiveStop);
	}
}

void off(int motor)
{
	Private::BattleHill::instance()->stop(motor);
}

void alloff()
{
	for (unsigned int i = 0; i < 4; ++i) off(i);
}

void ao()
{
	alloff();
}

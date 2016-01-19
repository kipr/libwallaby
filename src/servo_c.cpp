/*
 * servo_c.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/servo.h"
#include "wallaby_p.hpp"

void enable_servo(int port)
{
	set_servo_enabled(port, 1);
}

void disable_servo(int port)
{
	set_servo_enabled(port, 0);
}

void enable_servos()
{
	for (int i = 0; i < 4; ++i) enable_servo(i);
}

void disable_servos()
{
	for (int i = 0; i < 4; ++i) disable_servo(i);
}

void set_servo_enabled(int port, int enabled)
{
	Private::BattleHill::instance()->setServoEnabled(port, enabled ? true : false);
}

int get_servo_enabled(int port)
{
	return Private::BattleHill::instance()->getServoEnabled(port) ? 1 : 0;
}

int get_servo_position(int port)
{
	return Private::BattleHill::instance()->getServoPosition(port);
}

void set_servo_position(int port, int position)
{
	Private::BattleHill::instance()->setServoPosition(port, position);
}

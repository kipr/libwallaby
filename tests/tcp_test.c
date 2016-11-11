/*
 * sim_test.c
 *
 *  Created on: Nov 10, 2016
 *      Author: Joshua Southerland
 */

#include <stdio.h>

#include "wallaby/wallaby.h"


#define RM 3
#define LM 0
#define LIFT 0
#define GRIP 3

#define GRIP_OPEN 1500
#define GRIP_CLOSE 200

#define LIFT_DOWN 1200
#define LIFT_UP 1600

int main(int argc, char ** argv)
{

	// use the simulator, not the actual robot
	set_robot_sim_setting(1);

	// enable servos
	set_servo_enabled(LIFT, 1);
	set_servo_enabled(GRIP, 1);

	// claw open enough to miss can
	set_servo_position(GRIP, GRIP_OPEN);
	set_servo_position(LIFT, LIFT_DOWN);

	// drive forward
	mav(LM,100);
	mav(RM,100);
	msleep(15000);

	// stop
	mav(LM, 0);
	mav(RM, 0);

	// grap can
	set_servo_position(GRIP, GRIP_CLOSE);
	msleep(500);

	// lift can
	set_servo_position(LIFT, LIFT_UP);
	msleep(500);

	// drive backwards
	mav(LM,-100);
	mav(RM,-100);
	msleep(17000);

	// stop
	mav(LM, 0);
	mav(RM, 0);

	// set the can down
	set_servo_position(LIFT, LIFT_DOWN);
	msleep(1000);

	// release the can!
	set_servo_position(GRIP, GRIP_OPEN);
	msleep(500);

	return 0;
}

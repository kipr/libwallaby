/*
 * servo_test.c
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>

#include "wallaby/wallaby.h"

#define NUM_SERVOS 4
#define NUM_SERVO_CMDS 6

int main(int argc, char ** argv)
{
	unsigned short servo_cmds[6] = {0, 340, 682, 1024, 682, 340};

	// enable servos
	int i;
	for (i = 0; i < NUM_SERVOS; ++i) set_servo_enabled(i, 1);

	for (i = 0; i < 30; ++i)
	{
		int chan;
		for (chan = 0; chan < 4; ++chan)
		{
			unsigned short cmd_ind = (i + chan) % 6;
			set_servo_position(0, servo_cmds[cmd_ind]);
		}

		// TODO: demo allstop

		// TODO: demo individual enable/disable

		sleep(2);
	}

	// disable servos
	for (i = 0; i < NUM_SERVOS; ++i) set_servo_enabled(i, 0);

	return 0;
}

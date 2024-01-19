#include <stdio.h>

#include "kipr/servo/servo.h"
#include "kipr/time/time.h"

#define NUM_SERVOS 4
#define NUM_SERVO_CMDS 6

int main(int argc, char ** argv)
{
	unsigned short servo_cmds[6] = {0, 680, 1364, 2046, 1364, 680};

	// enable servos
	int i;
	for (i = 0; i < NUM_SERVOS; ++i) set_servo_enabled(i, 1);

	for (i = 0; i < 30; ++i)
	{
		int chan;
		for (chan = 0; chan < 4; ++chan)
		{
			unsigned short cmd_ind = (i + chan) % 6;
			set_servo_position(chan, servo_cmds[cmd_ind]);
		}

		// TODO: demo allstop

		// TODO: demo individual enable/disable

		msleep(2000);
	}

	// disable servos
	for (i = 0; i < NUM_SERVOS; ++i) set_servo_enabled(i, 0);

	return 0;
}

/*
 * motor_mav_loop_test.c
 *
 *  Created on: Dev 30, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{
	int velocity = 25;

	printf("forward at velocity %d on all four motors, in an infinite loop...\n", velocity);

	while(1)
	{
		int i;

		for (i = 0; i < 4; ++i) mav(i, velocity);
	}

	return 0;
}

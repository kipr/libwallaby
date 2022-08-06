/*
 * motor_mav_test.c
 *
 *  Created on: Nov 9, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{

	while(1)
	{
		int i;

		printf("forward\n");
		for (i = 0; i < 4; ++i) mav(i, 100);
		msleep(10000);

		printf("backward\n");
		for (i = 0; i < 4; ++i) mav(i, -100);
		msleep(10000);
	}

	return 0;
}

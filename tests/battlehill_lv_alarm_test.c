/*
 * comm_speed_test.c
 *
 *  Created on: Nov 4, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "wallaby/wallaby.h"

long elapsed_usec(struct timeval t1, struct timeval t2)
{
	return 1000000*(t2.tv_sec-t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
}


int main(int argc, char ** argv)
{
	const float vlevel_one = 5.5f;
	const float vlevel_two = 9.0f;

	while(1)
	{
		printf("set_low_volt_threshold(%f)\n", vlevel_one);
		set_low_volt_threshold(vlevel_one);
		sleep(20);

		printf("set_low_volt_threshold(%f)\n", vlevel_two);
		set_low_volt_threshold(vlevel_two);
		sleep(20);
	}

	return 0;
}

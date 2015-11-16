/*
 * create_test.c
 *
 *  Created on: Nov 16, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>

#include "wallaby/wallaby.h"


int main(int argc, char ** argv)
{
	int i;

	printf("Connecting...\n");
	create_connect();

	create_start();

	printf("Spinning CW...n");
	create_spin_CW(40);

	// spin a while CW
	for (i = 0; i < 10; ++i)
	{
		printf("Distance: %d  Angle: %d\n", get_create_distance(), get_create_total_angle());
		msleep(1000);
	}

	printf("Spinning CCW...n");
	create_spin_CCW(40);

	// spin a while CCW
	for (i = 0; i < 10; ++i)
	{
		printf("Distance: %d  Angle: %d\n", get_create_distance(), get_create_total_angle());
		msleep(1000);
	}


	printf("Stopping...\n");
	create_stop();

	printf("Disconnecting...\n");
	create_disconnect();

	printf("Done.\n");

	return 0;
}



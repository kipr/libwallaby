/*
 * motor_mtp_test.c
 *
 *  Created on: Nov 9, 2015
 *      Author: Joshua Southerland
 */


#include <stdio.h>

#include "wallaby/wallaby.h"

void report_position_counters()
{
	int j;
	int counts[4];
	for (j = 0; j < 4; ++j)
	{
		counts[j] = get_motor_position_counter(j);
	}

	printf("%d\t%d\t%d\t%d\n", counts[0], counts[1], counts[2], counts[3]);
}

int main(int argc, char ** argv)
{
	const int goal_one = 410000;
	const int goal_two = 0;

	const int speed_one = 300;
	const int speed_two = 800;

	while(1)
	{
		int i,j;
		printf("%d @ %d\n", goal_one, speed_one);
		for (i = 0; i < 4; ++i) mtp(i, speed_one, goal_one);

		// report position counts while we move
		for (i = 0; i < 10; ++i)
		{
			msleep(1000);
			report_position_counters();
		}

		printf("%d @ %d\n", goal_two, speed_two);
		for (i = 0; i < 4; ++i) mtp(i, speed_two, goal_two);

		// report position counts while we move
		for (i = 0; i < 10; ++i)
		{
			msleep(1000);
			report_position_counters();
		}
	}

	return 0;
}



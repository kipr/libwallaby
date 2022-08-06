/*
 * motor_mrp_test.c
 *
 *  Created on: Nov 10, 2015
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
	const int delta_one = 200000;
	const int delta_two = -200000;

	const int speed_one = 400;
	const int speed_two = 800;

	while(1)
	{
		int i,j;
		printf("%d @ %d\n", delta_one, speed_one);
		for (i = 0; i < 4; ++i) mrp(i, speed_one, delta_one);

		// report position counts while we move
		for (i = 0; i < 10; ++i)
		{
			msleep(1000);
			report_position_counters();
		}

		printf("%d @ %d\n", delta_two, speed_two);
		for (i = 0; i < 4; ++i) mrp(i, speed_two, delta_two);

		// report position counts while we move
		for (i = 0; i < 10; ++i)
		{
			msleep(1000);
			report_position_counters();
		}
	}

	return 0;
}

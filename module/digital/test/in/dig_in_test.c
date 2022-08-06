/*
 * dig_in_test.c
 *
 *  Created on: Nov 19, 2015
 *      Author: Joshua Southerland
 */



#include <stdio.h>

#include "wallaby/wallaby.h"

#define NUM_DIG_INS 16

int main(int argc, char ** argv)
{
	int i;
	for (i = 0; i < NUM_DIG_INS; ++i)
	{
		set_digital_output(i, 0);
	}

	while(1)
	{
		msleep(1000);

		for (i = 0; i < NUM_DIG_INS; ++i)
		{
			printf("%d  ", get_digital_value(i));
		}
		printf("\n");
	}

	return 0;
}


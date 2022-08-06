/*
 * dig_test.c
 *
 *  Created on: Nov 23, 2015
 *      Author: Joshua Southerland
 */


#include <stdio.h>

#include "wallaby/wallaby.h"


int main(int argc, char ** argv)
{
	// first 5 digital pins will be outputs
	int i;
	for (i = 0; i < 5; ++i)
	{
		set_digital_output(i, 1);
	}

	// next 5 will be inputs
	for (i = 5; i < 10; ++i)
	{
		set_digital_output(i, 0);
	}

	int on = 0;

	while(1)
	{

		// set pins on/off
		on ^= 1;
		for (i = 0; i < 5; ++i)
		{
			set_digital_value(i, on);
		}

		// wait so we don't display too often
		msleep(1000);

		for (i = 0; i < 10; ++i)
		{
			printf("%d  ", get_digital_value(i));
		}
		printf("\n");
	}

	return 0;
}

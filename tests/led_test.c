/*
 * led_test.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>

#include "wallaby/digital.h"

int main(int argc, char ** argv)
{
	set_digital_output(12, 1);
	usleep(250000);

	while(1)
	{
		usleep(250000);
		set_digital_value(12,1);

		usleep(250000);
		set_digital_value(12,0);
	}

	return 0;
}

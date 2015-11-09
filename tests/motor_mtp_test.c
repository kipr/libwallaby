/*
 * motor_mtp_test.c
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

		printf("1,000,000\n");
		for (i = 0; i < 4; ++i) mtp(i, 1000000, 300);
		msleep(10000);

		printf("0\n");
		for (i = 0; i < 4; ++i) mtp(i, 0, 800);
		msleep(10000);
	}

	return 0;
}



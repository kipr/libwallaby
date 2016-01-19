/*
 * get_pid_gains_test.c
 *
 *  Created on: Jan 18, 2016
 *      Author: Joshua Southerland
 */

#include "wallaby/wallaby.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
	short p = 0;
	short i = 0;
	short d = 0;
	short pd = 0;
	short id = 0;
	short dd = 0;

	int channel = 0;

	get_pid_gains(channel, &p, &i, &d, &pd, &id, &dd);

	printf("pid gains channel %d =  P(%d/%d) I(%d/%d) D(%d/%d)\n", channel, p, pd, i, id, d, dd);

	return 0;
}

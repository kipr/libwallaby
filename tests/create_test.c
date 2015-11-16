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
	printf("Connecting...\n");
	create_connect();

	create_start();

	printf("Spinning CW...n");
	create_spin_CW(40);

	// spin a while
	msleep(10000);

	printf("Stopping...\n");
	create_stop();

	printf("Disconnecting...\n");
	create_disconnect();

	printf("Done.\n");

	return 0;
}



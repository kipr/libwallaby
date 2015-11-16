/*
 * button_test.c
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{
	printf("Waiting for 'side' button to be pressed...\n");

	wait_for_side_button_clicked();

	printf("Bingo!... closing program.\n");

	return 0;
}



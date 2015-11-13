/*
 * ir_test.c
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */


#include <stdio.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{
	printf("Testing deprecated IR functions... don't expect much...\n");

	ir_read();

	ir_write();

	return 0;
}


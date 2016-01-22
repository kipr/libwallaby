/*
 * graphics_test.c
 *
 *  Created on: Jan 22, 2016
 *      Author: Joshua Southerland
 */

#include "wallaby/wallaby.h"


int main(int argc, char ** argv)
{
	graphics_open(100,100);

	graphics_line(0,0,49,49, 255, 0, 0);

	graphics_update();

	msleep(5000);

	graphics_close();

	return 0;
}



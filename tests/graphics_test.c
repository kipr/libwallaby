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

	int x, j;
	for (j = 0; j < 10; ++j)
	{
		for (x = 0; x < 100; ++x)
		{
			graphics_clear();
			graphics_circle_fill(x, 50, 5, 255, 0, 0);
			graphics_update();
			msleep(10);
		}
	}

	graphics_close();

	return 0;
}



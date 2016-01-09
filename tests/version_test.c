/*
 * battery_test.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{
	int i;
    for (i = 0; i < 10; ++i)
    {
        printf("%d\n",get_robot_firmware_version());
        msleep(1000);
    }

    return 0;
}

/*
 * battery_test.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>

#include "wallaby/battery.h"

int main(int argc, char ** argv)
{
    while(1)
    {
        float v = power_level();
        printf("%f\n",v);
        usleep(250000);
    }

    return 0;
}

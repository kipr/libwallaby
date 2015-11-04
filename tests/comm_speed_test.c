/*
 * comm_speed_test.c
 *
 *  Created on: Nov 4, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "wallaby/battery.h"

long elapsed_usec(struct timeval t1, struct timeval t2)
{
	return 1000000*(t2.tv_sec-t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
}

int main(int argc, char ** argv)
{
	set_digital_output(12, 1);

    while(1)
    {
        float v = power_level();
        if (v < 6.0 || v > 7.5) printf("%f\n",v);
    }

    return 0;
}

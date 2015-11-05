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

#include "wallaby/wallaby.h"

long elapsed_usec(struct timeval t1, struct timeval t2)
{
	return 1000000*(t2.tv_sec-t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
}

int main(int argc, char ** argv)
{
	set_digital_output(12, 1);

    while(1)
    {
    	struct timeval start, stop;
    	static const unsigned int num_samples = 100;

    	gettimeofday(&start, NULL);

    	unsigned int i;
    	for (i = 0; i < num_samples; ++i)
    	{
    		float v = power_level();
        	if (v < 6.0 || v > 7.5)
			{
        		printf("%f\n",v);
			}
    	}

    	gettimeofday(&stop, NULL);

    	long elapsed_time = elapsed_usec(start, stop);
    	double avg_sample_time_usec = ((double)elapsed_time)/((double)num_samples);

    	printf("%f uS per sample average (%f Hz)\n", avg_sample_time_usec, (1000000.0/avg_sample_time_usec));
    }

    return 0;
}

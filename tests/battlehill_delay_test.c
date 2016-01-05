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

void run_test()
{
	unsigned int i;
	for (i = 0; i < 10; ++i)
	{
		struct timeval start, stop;
		unsigned long int samples_start, samples_stop, num_samples;
		unsigned long int pubs_start, pubs_stop, num_pubs;

		gettimeofday(&start, NULL);

		samples_start = get_robot_update_count();
		pubs_start = get_robot_states_sequence_num();

		usleep(1000000);

		samples_stop = get_robot_update_count();
		pubs_stop = get_robot_states_sequence_num();

		gettimeofday(&stop, NULL);

		num_samples = samples_stop - samples_start;
		num_pubs = pubs_stop - pubs_start;

		long elapsed_time = elapsed_usec(start, stop);
		double avg_sample_time_usec = ((double)elapsed_time)/((double)num_samples);

		printf("%zu samples %zu publishes:   %f uS per sample average (%f Hz)\n", num_samples, num_pubs, avg_sample_time_usec, (1000000.0/avg_sample_time_usec));
	}
}


int main(int argc, char ** argv)
{
	const unsigned int delay_one = 1;
	const unsigned int delay_two = 1000;

	while(1)
	{
		printf("set_robot_update_delay(%d)\n", delay_one);
		set_robot_update_delay(delay_one);
		usleep(100000);
		run_test();

		printf("set_robot_update_delay(%d)\n", delay_two);
		set_robot_update_delay(delay_two);
		usleep(100000);
		run_test();
	}

	return 0;
}

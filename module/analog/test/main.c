#include <stdio.h>
#include <unistd.h>

#include "kipr/analog/analog.h"
#include "kipr/time/time.h"

#define NUM_ADC_CHANS 6

int main(int argc, char ** argv)
{
	int vals[NUM_ADC_CHANS];
	unsigned int count = 0;

	while(1)
	{
		count += 1;
		int channel;

		for (channel = 0; channel < NUM_ADC_CHANS; ++channel)
		{
			vals[channel] = analog(channel);
		}
		printf("analog values: %d %d %d %d %d %d\n", vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]);

		msleep(100);
	}

	return 0;
}

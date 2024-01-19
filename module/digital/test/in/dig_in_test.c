#include <stdio.h>

#include "kipr/digital/digital.h"
#include "kipr/time/time.h"

#define NUM_DIG_INS 16

int main(int argc, char ** argv)
{
	int i;
	for (i = 0; i < NUM_DIG_INS; ++i)
	{
		set_digital_output(i, 0);
	}

	while(1)
	{
		msleep(1000);

		for (i = 0; i < NUM_DIG_INS; ++i)
		{
			printf("%d  ", get_digital_value(i));
		}
		printf("\n");
	}

	return 0;
}


#include <stdio.h>

#include "kipr/compass/compass.h"
#include "kipr/time/time.h"

int main(int argc, char ** argv)
{
	// Test:  void calibrate_compass()
	// spin and calibrate the compass
	// this also displays the compass calibration values
	calibrate_compass();

	// Test:  float get_compass_angle()
	int i;
	const int stop = 60;
	for (i = 0; i < stop; ++i)
	{
		float angle = get_compass_angle();
		printf("%d/%d angle = %f\n", i+1, stop, angle);
		msleep(1000);
	}


	// Test:  void set_compass_params(float meanX, float meanY, float meanZ,
	//			    float W1, float W2, float div_E1, float div_E2)
	set_compass_params(-70.386002, -6.933000, 114.002998, -0.068338, 0.115273, 1.002332, 1.011292);


	printf("Done.\n");

	return 0;
}

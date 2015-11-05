/*
 * imu_test.c
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>
#include <unistd.h>

#include "wallaby/wallaby.h"


int main(int argc, char ** argv)
{
	while(1)
	{
		signed short ax = accel_x();
		signed short ay = accel_y();
		signed short az = accel_z();

		signed short gx = gyro_x();
		signed short gy = gyro_y();
		signed short gz = gyro_z();


		printf("ax:%d  ay:%d az:%d   gx:%d  gy:%d gz:%d\n",
				ax,	ay, az,
				gx, gy, gz);

		usleep(100000);
	}

	return 0;
}

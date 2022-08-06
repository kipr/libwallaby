/*
 * imu_test.c
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include <stdio.h>

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

		signed short mx = magneto_x();
		signed short my = magneto_y();
		signed short mz = magneto_z();

		printf("ax:%d ay:%d az:%d   gx:%d gy:%d gz:%d   mx:%d my:%d mz:%d\n",
				ax,	ay, az,
				gx, gy, gz,
				mx, my, mz);

		msleep(100);
	}

	return 0;
}

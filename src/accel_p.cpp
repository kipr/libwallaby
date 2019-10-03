/*
 * accel.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "accel_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"
#include "kipr/util.h"
#include "kipr/motors.h"
double biasx = 0, biasy = 0, biasz = 0;

namespace Private
{

short accel_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_X_H, alt_read_buffer))/16-biasx;
}

short accel_y(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Y_H, alt_read_buffer))/16-biasy;
}

short accel_z(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Z_H, alt_read_buffer))/16-biasz;
}

//Simple low-pass filter for accelerometer
bool accel_calibrate(int samples = 50)
{
  ao();
    msleep(300);
    
    //Find the average noise
    int i = 0;
    double avg = 0;
    while(i<samples){
     	avg += accel_z();
        msleep(10);
        i++;
    }
    biasz = avg/samples+512; //Z axis should be detecting gravity
	
    i = 0;
    avg = 0;
    while(i<samples){
     	avg += accel_y();
        msleep(10);
        i++;
    }
    biasy = avg/samples;
	
	i = 0;
    avg = 0;
    while(i<samples){
     	avg += accel_x();
        msleep(10);
        i++;
    }
    biasx = avg/samples;

    return 0;
}

}

/*
 * gyro.cpp
 *
 *  Created on: Dec 31, 2015 
 *	Last Updated: January 21, 2020
 *  Author(s): Joshua Southerland, Zachary Sasser
 */

#include "gyro_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"
#include "kipr/util.h"
#include "kipr/motors.h"


//Biases default to zero so that the low pass only takes affect if calibration is run.
int biasx = 0, biasy = 0, biasz = 0;

namespace Private
{
//Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short gyro_x(unsigned char * alt_read_buffer)
{
   return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_X_H, alt_read_buffer))/16-biasx;
}
	
//Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short gyro_y(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Y_H, alt_read_buffer))/16-biasy;
}
	
//Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short gyro_z(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Z_H, alt_read_buffer))/16-biasz;
}

//Simple low-pass filter for gyroscope
bool gyro_calibrate()
{
    int samples = 50;
    ao();
    msleep(300);
    
    //Find the average noise
	
    //Get the bias for the Z axis by sampling the stationary output.
    int i = 0;
    double avg = 0;
    while(i<samples){
     	avg += gyro_z();
        msleep(10);
        i++;
    }
    biasz += avg/samples;
	
    //Get the bias for the Y axis by sampling the stationary output.
    i = 0;
    avg = 0;
    while(i<samples){
     	avg += gyro_y();
        msleep(10);
        i++;
    }
    biasy += avg/samples;
	
    //Get the bias for the X axis by sampling the stationary output.
    i = 0;
    avg = 0;
    while(i<samples){
     	avg += gyro_x();
        msleep(10);
        i++;
    }
    biasx += avg/samples;
    printf("Bias Z: %d | Bias Y: %d | Bias X: %d \n", biasz,biasy,biasx);
    return 0;
}

// This function is currently unused and meaningless...
bool gyro_calibrated(unsigned char * alt_read_buffer)
{
  // TODO
  return true;
}


}

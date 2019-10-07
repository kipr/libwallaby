/*
 * gyro.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "gyro_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"
#include "kipr/util.h"
#include "kipr/motors.h"

double biasx = 0, biasy = 0, biasz = 0;
namespace Private
{

short gyro_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_X_L, alt_read_buffer))/16-biasx;
}

short gyro_y(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Y_L, alt_read_buffer))/16-biasy;
}

short gyro_z(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Z_L, alt_read_buffer))/16-biasz;
}

//Simple low-pass filter for gyroscope
bool gyro_calibrate()
{
    int samples = 50;
    ao();
    msleep(300);
    
    //Find the average noise
    int i = 0;
    double avg = 0;
    while(i<samples){
     	avg += gyro_z();
        msleep(10);
        i++;
    }
    biasz = avg/samples;
	
	i = 0;
    avg = 0;
    while(i<samples){
     	avg += gyro_y();
        msleep(10);
        i++;
    }
    biasy = avg/samples;
	
	i = 0;
    avg = 0;
    while(i<samples){
     	avg += gyro_x();
        msleep(10);
        i++;
    }
    biasx = avg/samples;

    return 0;
}

bool gyro_calibrated(unsigned char * alt_read_buffer)
{
  // TODO
  return true;
}


}

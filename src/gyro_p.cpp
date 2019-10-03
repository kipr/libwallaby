/*
 * gyro.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "gyro_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"
double bias = 0;
namespace Private
{

short gyro_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_X_L, alt_read_buffer))/16-bias;
}

short gyro_y(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Y_L, alt_read_buffer))/16-bias;
}

short gyro_z(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Z_L, alt_read_buffer))/16-bias;
}

bool gyro_calibrate()
{
  ao();
    msleep(300);
    
    //Find the average noise
    int i = 0;
    double avg = 0;
    while(i<50){
     	avg += gyro_z();
        msleep(1);
        i++;
    }
    bias = avg/50.0;

    return 0;
}


}

/*
 * gyro.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "gyro_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

short gyro_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_X_H, alt_read_buffer));
}

short gyro_y(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Y_H, alt_read_buffer));
}

short gyro_z(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Z_H, alt_read_buffer));
}

bool gyro_calibrate()
{
  // TODO
  return true;
}

bool gyro_calibrated(unsigned char * alt_read_buffer)
{
  // TODO
  return true;
}


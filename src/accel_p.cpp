/*
 * accel.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "accel_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

short accel_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_X_H, alt_read_buffer));
}

short accel_y(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Y_H, alt_read_buffer));
}

short accel_z(unsigned char * alt_read_buffer)
{
	return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Z_H, alt_read_buffer));
}

bool accel_calibrate()
{
  // TODO
  return true;
}

bool accel_calibrated(unsigned char * alt_read_buffer)
{
  // TODO
  return true;
}

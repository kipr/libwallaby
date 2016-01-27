/*
 * magneto_p.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "magneto_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

namespace Private
{

short magneto_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_X_H, alt_read_buffer))/16;
}

short magneto_y(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_Y_H, alt_read_buffer))/16;
}

short magneto_z(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_Z_H, alt_read_buffer))/16;
}

bool magneto_calibrate()
{
  // TODO
  return true;
}

bool magneto_calibrated(unsigned char * alt_read_buffer)
{
  // TODO
  return true;
}

}

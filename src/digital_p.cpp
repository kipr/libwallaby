/*
 * digital_p.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "digital_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

namespace Private{

bool digital_value(unsigned int port, unsigned char * alt_read_buffer)
{
  unsigned short dig_ins_val =  Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_IN_H, alt_read_buffer);

  // TODO: what if this isn't a digital in
  bool ret = dig_ins_val & (1 << port);
  return ret;
}

bool set_digital_value(unsigned char port, bool value)
{
  unsigned short out = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OUT_H);

  if (value)
  {
    out |= (1 << port);
  }
  else
  {
    out &= ~(1 << port);
  }

  Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OUT_H, out);

  return true; //TODO: return based on success
}


bool digital_output(unsigned char port, unsigned char * alt_read_buffer)
{
  // TODO: bounds check, if not a good port return Digital::Unknown
  unsigned short outputs = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OE_H, alt_read_buffer);
  bool output  = outputs & (1 << port);
  return output;
}


bool set_digital_direction(unsigned char port, bool output)
{
  // TODO: bounds check, if not a good port return Digital::Unknown
  unsigned short outputs = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OE_H);

  if (output)
  {
    outputs |= (1 << port);
  }
  else
  {
    outputs &= ~(1 << port);
  }

  Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OE_H, outputs);

  return true; // TODO: based on success
}
}


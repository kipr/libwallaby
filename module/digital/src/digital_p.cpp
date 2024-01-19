/*
 * digital_p.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "digital_p.hpp"

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

using namespace kipr;
using namespace kipr::digital;

using kipr::core::Platform;

bool kipr::digital::digital_value(unsigned int port)
{
  unsigned short dig_ins_val = Platform::instance()->readRegister16b(REG_RW_DIG_IN_H);

  // TODO: what if this isn't a digital in
  bool ret = dig_ins_val & (1 << port);
  return ret;
}

bool kipr::digital::set_digital_value(unsigned char port, bool value)
{
  unsigned short out = Platform::instance()->readRegister16b(REG_RW_DIG_OUT_H);

  if (value)
  {
    out |= (1 << port);
  }
  else
  {
    out &= ~(1 << port);
  }

  Platform::instance()->writeRegister16b(REG_RW_DIG_OUT_H, out);

  return true; //TODO: return based on success
}


bool kipr::digital::digital_output(unsigned char port)
{
  // TODO: bounds check, if not a good port return Digital::Unknown
  unsigned short outputs = Platform::instance()->readRegister16b(REG_RW_DIG_OE_H);
  bool output  = outputs & (1 << port);
  return output;
}


bool kipr::digital::set_digital_direction(unsigned char port, bool output)
{
  // TODO: bounds check, if not a good port return Digital::Unknown
  unsigned short outputs = Platform::instance()->readRegister16b(REG_RW_DIG_OE_H);

  if (output)
  {
    outputs |= (1 << port);
  }
  else
  {
    outputs &= ~(1 << port);
  }

  Platform::instance()->writeRegister16b(REG_RW_DIG_OE_H, outputs);

  return true; // TODO: based on success
}
/*
 * battery_p.cpp
 *
 *  Created on: Dec 30, 2015
 *      Author: Joshua Southerland
 */

#include "battery_p.hpp"

#include "wallaby_regs_p.hpp"
#include "wallaby_p.hpp"

namespace Private
{

unsigned short battery_raw_reading(unsigned char * alt_read_buffer)
{
  return Private::Wallaby::instance()->readRegister16b(REG_RW_BATT_H, alt_read_buffer);
}

float battery_power_level(unsigned short raw_batt)
{
  // calculate voltage based on linear curve-fitting
  float batt_voltage = -0.02070635f + 0.009071161f * static_cast<float>(raw_batt);

  // convert to capacity
  // TODO: better calibration
  float empty_voltage = 5.5f;
  float full_voltage = 7.15f;
  float voltage_range = full_voltage - empty_voltage;
  float perc = (batt_voltage - empty_voltage) / voltage_range;
  if (perc > 1.0f) perc = 1.0f;
  if (perc < 0.0f) perc = 0.0f;

  return perc;
}

}

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

// linear interpolation
float interp(float x, float xLo, float xHi, float yLo, float yHi)
{
	float yRange = yHi - yLo;
	float xRange = xHi - xLo;
	return (yRange * ((x - xLo) / xRange) + yLo);
}

float battery_power_level(unsigned short raw_batt)
{
  // calculate voltage based on linear curve-fitting
  double voltage =  0.009175 * static_cast<double>(raw_batt);

  static const unsigned int NUM_SAMPLES = 5;
  const float vs[NUM_SAMPLES] = {5.650, 6.413, 6.652, 6.707, 6.760}; // voltage
  const float ps[NUM_SAMPLES] = {0.000, 0.065, 0.420, 0.760, 1.000}; // percentage / 100

  if (voltage < (vs[0] + 0.001)) return 0.0f;

  for (unsigned int i = 0; i < NUM_SAMPLES-1; ++i)
  {
	  if (voltage < vs[i+1]) return interp(voltage, vs[i], vs[i+1], ps[i], ps[i+1]);
  }
  return 1.0f;
}

}

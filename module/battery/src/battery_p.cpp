#include "battery_p.hpp"

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

using namespace kipr;
using namespace kipr::battery;

using kipr::core::Platform;

unsigned short kipr::battery::battery_raw_reading()
{
  return Platform::instance()->readRegister16b(REG_RW_BATT_H);
}

// linear interpolation
float interp(float x, float xLo, float xHi, float yLo, float yHi)
{
  float yRange = yHi - yLo;
  float xRange = xHi - xLo;
  return (yRange * ((x - xLo) / xRange) + yLo);
}

// interpolate two arrays of size num_pts based on voltage
// returns ps[0] if the query point falls before the sample points, or ps[num_pts-1] if it is after
float voltage_to_capacity(const float *vs, const float *ps, unsigned int num_pts, float voltage)
{
  if (voltage < (vs[0] + 0.001))
    return 0.0f;

  for (unsigned int i = 0; i < num_pts - 1; ++i)
  {
    if (voltage < vs[i + 1])
      return interp(voltage, vs[i], vs[i + 1], ps[i], ps[i + 1]);
  }

  return ps[num_pts - 1];
}

float kipr::battery::battery_power_level(Type batt_type, unsigned short raw_batt)
{
  // calculate voltage based on linear curve-fitting
  double voltage = 0.009175 * static_cast<double>(raw_batt);

  // LiFePO4 calibrated using a Wallaby  controller at ~0.4A discharge and a 2S LiFePO4 Common-Sense RC battery
  static const unsigned int NUM_SAMPLES_LIFE = 5;
  const float vs_LiFe[NUM_SAMPLES_LIFE] = {5.650, 6.413, 6.652, 6.707, 6.760}; // voltage
  const float ps_LiFe[NUM_SAMPLES_LIFE] = {0.000, 0.065, 0.420, 0.760, 1.000}; // percentage / 100

  // LiPO taken from http://the-writers.club/winterwang/wp-content/uploads/sites/132/2016/01/LIPOCHART.jpg
  static const unsigned int NUM_SAMPLES_LIPO = 8;
  const float vs_LiPo[NUM_SAMPLES_LIPO] = {7.300, 7.350, 7.400, 7.500, 7.600, 7.700, 7.800, 8.400}; // voltage
  const float ps_LiPo[NUM_SAMPLES_LIPO] = {0.000, 0.100, 0.200, 0.400, 0.500, 0.600, 0.800, 1.000}; // percentage / 100

  // NiMH adapted from http://www.electro-tech-online.com/attachments/aa-ni-mh-cell-high-currents-png.48452/
  static const unsigned int NUM_SAMPLES_NIMH = 5;
  const float vs_NIMH[NUM_SAMPLES_NIMH] = {6.300, 6.600, 7.200, 7.800, 8.400}; // voltage
  const float ps_NIMH[NUM_SAMPLES_NIMH] = {0.000, 0.250, 0.500, 0.750, 1.000}; // percentage / 100

  switch (batt_type)
  {
  case Type::LiFe:
    return voltage_to_capacity(vs_LiFe, ps_LiFe, NUM_SAMPLES_LIFE, voltage);
  case Type::LiPo:
    return voltage_to_capacity(vs_LiPo, ps_LiPo, NUM_SAMPLES_LIPO, voltage);
  case Type::NiMH:
    return voltage_to_capacity(vs_NIMH, ps_NIMH, NUM_SAMPLES_NIMH, voltage);
  default:
    break;
  }

  return 0.0f; // shouldn't get to here
}

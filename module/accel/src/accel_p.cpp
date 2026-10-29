#include "accel_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/time/time.h"

#include <atomic>
#include <cmath>
#include <cstdio>

using namespace kipr;
using namespace kipr::accel;

using kipr::core::Platform;

namespace
{
  // Atomic because user programs may read the accelerometer on one thread while
  // another thread runs accel_calibrate().
  std::atomic<double> Biasx{0};
  std::atomic<double> Biasy{0};
  std::atomic<double> Biasz{0};

  // 1 g in output units: the firmware runs the accelerometer at +/-2 g, and the
  // raw register is divided by 16.
  const double kCountsPerG = 1024;

  // If any axis varies more than this (standard deviation, in output units)
  // while sampling, the controller was not still and calibration fails.
  // Stationary noise is a few counts; 30 counts is about 0.03 g.
  const double kMaxStillStdDev = 30;

  const int kSamples = 50;

  // Raw axis reading in output units. Integer division truncates toward zero,
  // as the getters always have, so uncalibrated readings are unchanged.
  int read_axis(unsigned char reg)
  {
    return static_cast<signed short>(Platform::instance()->readRegister16b(reg)) / 16;
  }

  struct AxisStats
  {
    double sum = 0;
    double sumSq = 0;

    void add(double v)
    {
      sum += v;
      sumSq += v * v;
    }

    double mean() const { return sum / kSamples; }

    double stddev() const
    {
      const double m = mean();
      const double var = sumSq / kSamples - m * m;
      return var > 0 ? std::sqrt(var) : 0;
    }
  };
}

short kipr::accel::accel_x()
{
  return std::lround(read_axis(REG_RW_ACCEL_X_H) - Biasx.load());
}

short kipr::accel::accel_y()
{
  return std::lround(read_axis(REG_RW_ACCEL_Y_H) - Biasy.load());
}

short kipr::accel::accel_z()
{
  return std::lround(read_axis(REG_RW_ACCEL_Z_H) - Biasz.load());
}

// Measures the at-rest offset of each axis and subtracts it from later readings.
// Returns false, keeping the previous calibration, if the controller moved.
//
// TODO: This assumes the controller is flat with gravity on -Z. It zeroes X and Y
// and forces Z to read -1 g whatever the real orientation, so on a controller
// mounted on its side the reported gravity vector ends up rotated (e.g. gravity on
// X reads as x = 0, z = -1024). Options to investigate: reject calibration when
// the measured gravity is not mostly on Z, or subtract only the sensor offset
// (needs a per-board offset measured in a known orientation) rather than gravity.
bool kipr::accel::accel_calibrate()
{
  // Sample raw readings; the existing calibration stays in effect meanwhile
  AxisStats x, y, z;
  for (int i = 0; i < kSamples; ++i)
  {
    x.add(read_axis(REG_RW_ACCEL_X_H));
    y.add(read_axis(REG_RW_ACCEL_Y_H));
    z.add(read_axis(REG_RW_ACCEL_Z_H));
    msleep(10);
  }

  if (x.stddev() > kMaxStillStdDev || y.stddev() > kMaxStillStdDev || z.stddev() > kMaxStillStdDev)
  {
    printf("[Accel Calibration]: Failed, controller moved during calibration (std dev Z: %f | Y: %f | X: %f)\n",
           z.stddev(), y.stddev(), x.stddev());
    return false;
  }

  const double biasx = x.mean();
  const double biasy = y.mean();
  const double biasz = z.mean() + kCountsPerG; // Z axis should read -1 g at rest

  Biasx = biasx;
  Biasy = biasy;
  Biasz = biasz;

  printf("[Accel Calibration]: Bias Z: %f | Bias Y: %f | Bias X: %f \n", biasz, biasy, biasx);
  return true;
}

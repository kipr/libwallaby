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

  // Accepted range for the total reading at rest, in g.
  const double kMinGravity = 0.75;
  const double kMaxGravity = 1.25;

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
// Gravity is assumed to lie along whichever axis reads largest, so this works
// with the controller flat, upside down or on any side. That axis keeps +/-1 g
// and the other two are zeroed; any tilt from that axis is absorbed into the
// offsets, as before. A flat controller gets the same biases as when this only
// supported gravity on -Z.
//
// Returns false, keeping the previous calibration, if the controller moved or the
// total reading is too far from 1 g to be gravity (e.g. a faulty sensor).
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

  const double m[3] = {x.mean(), y.mean(), z.mean()};

  // An in-spec sensor reads within about 0.89-1.11 g at rest (offset plus scale
  // error), so this only rejects readings that can't be gravity.
  const double mag = std::sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
  if (mag < kMinGravity * kCountsPerG || mag > kMaxGravity * kCountsPerG)
  {
    printf("[Accel Calibration]: Failed, total reading is %f g, not 1 g (Z: %f | Y: %f | X: %f)\n",
           mag / kCountsPerG, m[2], m[1], m[0]);
    return false;
  }

  // Gravity axis: the one with the largest reading
  int g = 0;
  for (int i = 1; i < 3; ++i)
    if (std::fabs(m[i]) > std::fabs(m[g]))
      g = i;

  double bias[3] = {m[0], m[1], m[2]};
  bias[g] -= std::copysign(kCountsPerG, m[g]); // that axis should read +/-1 g at rest

  Biasx = bias[0];
  Biasy = bias[1];
  Biasz = bias[2];

  printf("[Accel Calibration]: Bias Z: %f | Bias Y: %f | Bias X: %f \n", bias[2], bias[1], bias[0]);
  return true;
}

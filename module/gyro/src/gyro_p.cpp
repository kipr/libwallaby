#include "gyro_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/time/time.h"

#include <atomic>
#include <cmath>
#include <cstdio>

using namespace kipr;
using namespace kipr::gyro;

using kipr::core::Platform;

namespace
{
  // Biases default to zero so readings are uncorrected until calibration runs.
  // Atomic because user programs may read the gyro on one thread while another
  // thread runs gyro_calibrate(). Each bias is read whole, but the three are
  // stored one at a time.
  // TODO: a reader racing a recalibration can mix old and new biases across axes.
  // If that matters, publish all three as one snapshot (mutex, seqlock or a
  // single atomic struct). Same in accel_p.cpp.
  std::atomic<double> biasx{0};
  std::atomic<double> biasy{0};
  std::atomic<double> biasz{0};

  // If any axis varies more than this (standard deviation, in output units)
  // while sampling, the controller was rotating and calibration fails.
  // Stationary noise is under 1 count; 10 counts is a few degrees/sec.
  const double kMaxStillStdDev = 10;

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

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_x()
{
  return std::lround(read_axis(REG_RW_GYRO_X_H) - biasx.load());
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_y()
{
  return std::lround(read_axis(REG_RW_GYRO_Y_H) - biasy.load());
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_z()
{
  return std::lround(read_axis(REG_RW_GYRO_Z_H) - biasz.load());
}

// Measures the zero-rate offset of each axis and subtracts it from later readings.
// Returns false, keeping the previous calibration, if the controller moved.
bool kipr::gyro::gyro_calibrate()
{
  // Sample raw readings; the existing calibration stays in effect meanwhile
  AxisStats x, y, z;
  for (int i = 0; i < kSamples; ++i)
  {
    x.add(read_axis(REG_RW_GYRO_X_H));
    y.add(read_axis(REG_RW_GYRO_Y_H));
    z.add(read_axis(REG_RW_GYRO_Z_H));
    msleep(10);
  }

  if (x.stddev() > kMaxStillStdDev || y.stddev() > kMaxStillStdDev || z.stddev() > kMaxStillStdDev)
  {
    printf("[Gyro Calibrate]: Failed, controller moved during calibration (std dev Z: %f | Y: %f | X: %f)\n",
           z.stddev(), y.stddev(), x.stddev());
    return false;
  }

  const double bx = x.mean();
  const double by = y.mean();
  const double bz = z.mean();

  biasx = bx;
  biasy = by;
  biasz = bz;

  printf("[Gyro Calibrate]: Bias Z: %f | Bias Y: %f | Bias X: %f \n", bz, by, bx);
  return true;
}

// This function is currently unused and meaningless...
bool kipr::gyro::gyro_calibrated()
{
  // TODO
  return true;
}

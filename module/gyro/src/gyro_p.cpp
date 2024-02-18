#include "gyro_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/time/time.h"

namespace
{
  // Biases default to zero so that the low pass only takes affect if calibration is run.
  int biasx = 0;
  int biasy = 0;
  int biasz = 0;
}

using namespace kipr;
using namespace kipr::gyro;

using kipr::core::Platform;

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_x()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_X_H)) / 16 - biasx;
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_y()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_Y_H)) / 16 - biasy;
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_z()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_Z_H)) / 16 - biasz;
}

// Simple low-pass filter for gyroscope
bool kipr::gyro::gyro_calibrate()
{
  int samples = 50;

  // Find the average noise of the gyro. Get the bias for gyro axis by sampling the stationary output.
  int i = 0;
  double sumX = 0, sumY = 0, sumZ = 0;
  while (i < samples)
  {
    sumX += gyro_x();
    sumY += gyro_y();
    sumZ += gyro_z();
    msleep(10);
    i++;
  }

  biasx = sumX / samples;
  biasy = sumY / samples;
  biasz = sumZ / samples;

  printf("[Gyro Calibrate]: Bias Z: %d | Bias Y: %d | Bias X: %d \n", biasz, biasy, biasx);
  return 0;
}

// This function is currently unused and meaningless...
bool kipr::gyro::gyro_calibrated()
{
  // TODO
  return true;
}
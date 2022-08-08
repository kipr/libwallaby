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
short kipr::gyro::gyro_x(unsigned char *alt_read_buffer)
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_X_H, alt_read_buffer)) / 16 - biasx;
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_y(unsigned char *alt_read_buffer)
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_Y_H, alt_read_buffer)) / 16 - biasy;
}

// Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
short kipr::gyro::gyro_z(unsigned char *alt_read_buffer)
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_GYRO_Z_H, alt_read_buffer)) / 16 - biasz;
}

// Simple low-pass filter for gyroscope
bool kipr::gyro::gyro_calibrate()
{
  int samples = 50;

  // Find the average noise

  // Get the bias for the Z axis by sampling the stationary output.
  int i = 0;
  double avg = 0;
  while (i < samples)
  {
    avg += gyro_z();
    msleep(10);
    i++;
  }
  biasz += avg / samples;

  // Get the bias for the Y axis by sampling the stationary output.
  i = 0;
  avg = 0;
  while (i < samples)
  {
    avg += gyro_y();
    msleep(10);
    i++;
  }
  biasy += avg / samples;

  // Get the bias for the X axis by sampling the stationary output.
  i = 0;
  avg = 0;
  while (i < samples)
  {
    avg += gyro_x();
    msleep(10);
    i++;
  }
  biasx += avg / samples;
  printf("Bias Z: %d | Bias Y: %d | Bias X: %d \n", biasz, biasy, biasx);
  return 0;
}

// This function is currently unused and meaningless...
bool kipr::gyro::gyro_calibrated(unsigned char *alt_read_buffer)
{
  // TODO
  return true;
}
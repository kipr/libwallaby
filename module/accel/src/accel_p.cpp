#include "accel_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/time/time.h"

using namespace kipr;
using namespace kipr::accel;

using kipr::core::Platform;

namespace
{
  double Biasx = 0;
  double Biasy = 0;
  double Biasz = 0;
}

short kipr::accel::accel_x()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_ACCEL_X_H)) / 16 - Biasx;
}

short kipr::accel::accel_y()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_ACCEL_Y_H)) / 16 - Biasy;
}

short kipr::accel::accel_z()
{
  return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_ACCEL_Z_H)) / 16 - Biasz;
}

// Simple low-pass filter for accelerometer
bool kipr::accel::accel_calibrate()
{
  int samples = 50;

  // Find the average noise
  int i = 0;
  double sumX = 0, sumY = 0, sumZ = 0;
  while (i < samples)
  {
    sumX += accel_z();
    sumY += accel_y();
    sumZ += accel_x();

    msleep(10);
    i++;
  }

  Biasx = sumX / samples;
  Biasy = sumY / samples;
  Biasz = sumZ / samples + 512; // Z axis should be detecting gravity

  printf("[Accel Calibration]: Bias Z: %f | Bias Y: %f | Bias X: %f \n", Biasz, Biasy, Biasx);
  return 0;
}

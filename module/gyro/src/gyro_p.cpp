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
  bool calibrated = false;
}

using namespace kipr;
using namespace kipr::gyro;

using kipr::core::Platform;

/*
 * Outputs the internal gyro x reading
 * Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_gyro_x()
{
  return Platform::instance()->readRegisterFloat(REG_RW_GYRO_X_0);
}

/*
 * Outputs the internal gyro y reading
 * Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_gyro_y()
{
  return Platform::instance()->readRegisterFloat(REG_RW_GYRO_Y_0);
}

/*
 * Outputs the internal gyro z reading
 * Pulls gyroscope data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_gyro_z()
{
  return Platform::instance()->readRegisterFloat(REG_RW_GYRO_Z_0);
}


/*
 * Returns the gyro_x in the NED frame. +x is forward (North). -x is backward (South).
 */
short kipr::gyro::gyro_x()
{
  return static_cast<short>(-local_gyro_y());
}

/*
 * Returns the gyro_y in the NED frame. +y is right (East). -y is left (West).
 */
short kipr::gyro::gyro_y()
{
  return static_cast<short>(local_gyro_x());
}

/*
 * Returns the gyro_z in the NED frame. +z is up. -z is down.
 */
short kipr::gyro::gyro_z()
{
  return static_cast<short>(local_gyro_z());
}

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
  calibrated = true;
  return true;
}

// This function is currently unused and meaningless...
bool kipr::gyro::gyro_calibrated()
{
  return calibrated;
}
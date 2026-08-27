#ifndef _KIPR_GYRO_GYRO_P_HPP_
#define _KIPR_GYRO_GYRO_P_HPP_

namespace kipr
{
  namespace gyro
  {
    short gyro_x();

    short gyro_y();

    short gyro_z();

    bool gyro_calibrate();

    bool gyro_calibrated();
  }
}

#endif

#ifndef _KIPR_GYRO_GYRO_P_HPP_
#define _KIPR_GYRO_GYRO_P_HPP_

namespace kipr
{
  namespace gyro
  {
    short gyro_x(unsigned char * alt_read_buffer = nullptr);

    short gyro_y(unsigned char * alt_read_buffer = nullptr);

    short gyro_z(unsigned char * alt_read_buffer = nullptr);

    bool gyro_calibrate();

    bool gyro_calibrated(unsigned char * alt_read_buffer = nullptr);
  }
}

#endif

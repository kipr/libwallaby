#ifndef _KIPR_ACCEL_ACCEL_P_HPP_
#define _KIPR_ACCEL_ACCEL_P_HPP_

namespace kipr
{
  namespace accel
  {
    short accel_x(unsigned char *alt_read_buffer = nullptr);

    short accel_y(unsigned char *alt_read_buffer = nullptr);

    short accel_z(unsigned char *alt_read_buffer = nullptr);

    bool accel_calibrate();
  }
}

#endif

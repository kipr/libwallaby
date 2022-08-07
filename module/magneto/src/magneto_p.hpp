#ifndef _KIPR_MAGNETO_MAGNETO_P_HPP_
#define _KIPR_MAGNETO_MAGNETO_P_HPP_

namespace kipr
{
  namespace magneto
  {
    short magneto_x(unsigned char *alt_read_buffer = nullptr);
    short magneto_y(unsigned char *alt_read_buffer = nullptr);
    short magneto_z(unsigned char *alt_read_buffer = nullptr);
  }
}

#endif
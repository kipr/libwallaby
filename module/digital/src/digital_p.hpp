#ifndef _KIPR_DIGITAL_DIGITAL_P_H_
#define _KIPR_DIGITAL_DIGITAL_P_H_

namespace kipr
{
  namespace digital
  {
    // returns true if the port is high, false if low
    bool digital_value(unsigned int port);

    bool set_digital_value(unsigned char port, bool value);

    // returns true if the port is an output, or false if it isn't
    bool digital_output(unsigned char port);

    bool set_digital_direction(unsigned char port, bool output);
  }
}

#endif

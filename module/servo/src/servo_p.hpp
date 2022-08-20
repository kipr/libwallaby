#ifndef _KIPR_SERVO_SERVO_P_HPP_
#define _KIPR_SERVO_SERVO_P_HPP_

namespace kipr
{
  namespace servo
  {
    void set_servo_enabled(int port, bool enabled);
    bool get_servo_enabled(int port);
    bool set_servo_position(int port, unsigned short position);
    unsigned short get_servo_position(int port);
  }
}

#endif

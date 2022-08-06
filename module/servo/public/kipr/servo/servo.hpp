#ifndef _KIPR_SERVO_SERVO_HPP_
#define _KIPR_SERVO_SERVO_HPP_

namespace kipr
{
  namespace servo
  {
    class Servo
    {
    public:
      typedef unsigned int ticks_t;

      Servo(int port);

      void setPosition(ticks_t position);

      ticks_t position() const;

      void disable();
      void enable();
      void setEnabled(bool enabled);
      bool isEnabled() const;

    private:
      int m_port;
    };
  }
}

#endif

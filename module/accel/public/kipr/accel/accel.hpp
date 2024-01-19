#ifndef _KIPR_ACCEL_ACCEL_HPP_
#define _KIPR_ACCEL_ACCEL_HPP_

#include "kipr/sensor/sensor.hpp"

namespace kipr
{
  namespace accel
  {
    class Acceleration
    {
    public:
      static short x();
      static short y();
      static short z();
      static bool calibrate();

    private:
    };

    class AccelX : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };

    class AccelY : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };

    class AccelZ : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };
  }
}

#endif

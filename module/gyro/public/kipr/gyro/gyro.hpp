#ifndef _KIPR_GYRO_GYRO_HPP_
#define _KIPR_GYRO_GYRO_HPP_

#include "kipr/sensor/sensor.hpp"

namespace kipr
{
  namespace gyro
  {
    class Gyro
    {
    public:
      static short x();
      static short y();
      static short z();
      static bool calibrate();

    private:
    };

    class GyroX : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };

    class GyroY : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };

    class GyroZ : public sensor::Sensor<short>
    {
    public:
      virtual short value() const;
    };
  }
}

#endif

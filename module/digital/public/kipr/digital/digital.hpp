#ifndef _KIPR_DIGITAL_DIGITAL_HPP_
#define _KIPR_DIGITAL_DIGITAL_HPP_

#include "kipr/sensor/sensor.hpp"

namespace kipr
{
  namespace digital
  {
    class Digital : public sensor::Sensor<bool>
    {
    public:
      Digital(unsigned char port);

      void setValue(bool value);

      void setOutput(bool output);

      bool isOutput() const;

      void setPullup(bool pullup);

      bool pullup() const;

      virtual bool value() const;

    private:
      unsigned char m_port;
    };
  }
}

#endif

#ifndef _SENSOR_LOGIC_HPP_
#define _SENSOR_LOGIC_HPP_

#include "sensor.hpp"
#include "kipr/export/export.h"

namespace kipr
{
  namespace sensor
  {
    namespace logic
    {
      class EXPORT_SYM Base : public Sensor<bool>
      {
      public:
        Base(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
        ~Base();
        
        const Sensor<bool> *a() const;
        const Sensor<bool> *b() const;
        bool owns() const;
      private:
        const Sensor<bool> *m_a;
        const Sensor<bool> *m_b;
        bool m_owns;
      };
      
      class EXPORT_SYM And : public Base
      {
      public:
        And(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
        virtual bool value() const;
      };
      
      class EXPORT_SYM Or : public Base
      {
      public:
        Or(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
        virtual bool value() const;
      };
      
      class EXPORT_SYM Xor : public Base
      {
      public:
        Xor(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
        virtual bool value() const;
      };
      
      class EXPORT_SYM Not : public Sensor<bool>
      {
      public:
        Not(const Sensor<bool> *input, bool owns = false);
        ~Not();
        virtual bool value() const;
        
        const Sensor<bool> *input() const;
        bool owns() const;
        
      private:
        const Sensor<bool> *m_input;
        bool m_owns;
      };
    }
  }
}


#endif

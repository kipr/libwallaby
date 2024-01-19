#ifndef _KIPR_BATTERY_BATTERY_HPP_
#define _KIPR_BATTERY_BATTERY_HPP_

namespace kipr
{
  namespace battery
  {
    class Battery
    {
    public:
      static bool isCharging();
      static float powerLevel(unsigned int battery_type = 0);
      static unsigned short rawPowerADC();
    };
  }
}

#endif

#ifndef _KIPR_BATTERY_BATTERY_P_HPP_
#define _KIPR_BATTERY_BATTERY_P_HPP_

namespace kipr
{
  namespace battery
  {
    enum class Type
    {
      LiFe = 0,
      LiPo,
      NiMH
    };

    unsigned short battery_raw_reading();

    // TODO: move to battery source/header
    // TODO: doesn't allow for use of an alt_read_buffer
    // battery types, 0=LiFePO4, 1=LiPo, 2=NiMH
    float battery_power_level(Type batt_type = Type::LiFe, unsigned short raw_batt = battery_raw_reading());

  }
}

#endif

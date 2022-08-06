/*
 * battery_p.hpp
 *
 *  Created on: Dec 30, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_BATTERY_P_HPP_
#define SRC_BATTERY_P_HPP_

namespace Private
{

namespace Battery
{
  enum BatteryType
  {
    LiFe = 0,
    LiPo,
    NiMH
  };
}

unsigned short battery_raw_reading(unsigned char * alt_read_buffer = nullptr);

// TODO: move to battery source/header
// TODO: doesn't allow for use of an alt_read_buffer
// battery types, 0=LiFePO4, 1=LiPo, 2=NiMH
float battery_power_level(Battery::BatteryType batt_type = Battery::LiFe, unsigned short raw_batt = battery_raw_reading());

}

#endif /* SRC_BATTERY_P_HPP_ */

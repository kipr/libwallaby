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

unsigned short battery_raw_reading(unsigned char * alt_read_buffer = nullptr);

// TODO: move to battery source/header
// TODO: confusing, this should be % remaining, but currently returns a voltage approximation
// TODO: doesn't allow for use of an alt_read_buffer
float battery_power_level(unsigned short raw_batt = battery_raw_reading());

}

#endif /* SRC_BATTERY_P_HPP_ */

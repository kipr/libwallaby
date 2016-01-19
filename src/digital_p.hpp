/*
 * digital_p.hpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#ifndef SRC_DIGITAL_P_HPP_
#define SRC_DIGITAL_P_HPP_


namespace Private{

// returns true if the port is high, false if low
bool digital_value(unsigned int port, unsigned char * alt_read_buffer);

bool set_digital_value(unsigned char port, bool value);

// returns true if the port is an output, or false if it isn't
bool digital_output(unsigned char port, unsigned char * alt_read_buffer);

bool set_digital_direction(unsigned char port, bool output);

}

#endif /* SRC_DIGITAL_P_HPP_ */

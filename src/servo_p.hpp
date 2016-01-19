/*
 * servo_p.hpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#ifndef SRC_SERVO_P_HPP_
#define SRC_SERVO_P_HPP_

namespace Private
{

void set_servo_enabled(int port, bool enabled);

bool get_servo_enabled(int port, unsigned char * alt_read_buffer);

bool set_servo_position(int port, unsigned short position);

unsigned short get_servo_position(int port, unsigned char * alt_read_buffer);

}

#endif /* SRC_SERVO_P_HPP_ */

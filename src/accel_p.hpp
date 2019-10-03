/*
 * accel_p.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_ACCEL_P_HPP_
#define SRC_ACCEL_P_HPP_

namespace Private
{

short accel_x(unsigned char * alt_read_buffer = nullptr);

short accel_y(unsigned char * alt_read_buffer = nullptr);

short accel_z(unsigned char * alt_read_buffer = nullptr);

bool accel_calibrate();



}

#endif /* SRC_ACCEL_P_HPP_ */

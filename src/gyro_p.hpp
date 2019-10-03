/*
 * gyro_p.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_GYRO_P_HPP_
#define SRC_GYRO_P_HPP_

namespace Private
{

short gyro_x(unsigned char * alt_read_buffer = nullptr);

short gyro_y(unsigned char * alt_read_buffer = nullptr);

short gyro_z(unsigned char * alt_read_buffer = nullptr);

bool gyro_calibrate();


}

#endif /* SRC_GYRO_P_HPP_ */

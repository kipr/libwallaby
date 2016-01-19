/*
 * magneto_p.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_MAGNETO_P_HPP_
#define SRC_MAGNETO_P_HPP_

namespace Private
{


short magneto_x(unsigned char * alt_read_buffer = nullptr);

short magneto_y(unsigned char * alt_read_buffer = nullptr);

short magneto_z(unsigned char * alt_read_buffer = nullptr);

bool magneto_calibrate();

bool magneto_calibrated(unsigned char * alt_read_buffer = nullptr);

}


#endif /* SRC_MAGNETO_P_HPP_ */

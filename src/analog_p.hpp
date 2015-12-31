/*
 * analog_p.hpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_ANALOG_P_HPP_
#define SRC_ANALOG_P_HPP_

namespace Private
{

class Analog
{
public:
	~Analog();

	unsigned short value(unsigned char port) const;

	static Analog * instance();

private:
	Analog();
	Analog(const Analog & rhs);
	Analog & operator=(const Analog & rhs);
};

}

#endif /* SRC_ANALOG_P_HPP_ */

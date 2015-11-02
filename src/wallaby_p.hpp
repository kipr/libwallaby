/*
 * wallaby_p.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_WALLABY_P_HPP_
#define SRC_WALLABY_P_HPP_

namespace Private
{

class Wallaby
{

public:
	static Wallaby * instance();
	bool readRegister(unsigned short address, unsigned char & value) const;

	virtual ~Wallaby();

private:
	Wallaby();
	int spi_fd_;

};

} /* namespace Private */

#endif /* SRC_WALLABY_P_HPP_ */

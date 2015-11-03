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
	bool readRegister(unsigned short address, unsigned char & value);
	bool writeRegister(unsigned short address, unsigned char value);

	virtual ~Wallaby();

private:
	Wallaby();
	bool transfer();
	void clear_buffers();
	int spi_fd_;
	unsigned char * write_buffer_;
	unsigned char * read_buffer_;
	const unsigned int buffer_size_;

};

} /* namespace Private */

#endif /* SRC_WALLABY_P_HPP_ */

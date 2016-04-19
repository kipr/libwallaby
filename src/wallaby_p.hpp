/*
 * wallaby_p.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_WALLABY_P_HPP_
#define SRC_WALLABY_P_HPP_

#include <mutex>

namespace Private
{

extern std::mutex shutdown_mutex;

class Wallaby
{

public:
	static Wallaby * instance();

	unsigned char readRegister8b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
	void writeRegister8b(unsigned char address, unsigned char value);

	unsigned short readRegister16b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
	void writeRegister16b(unsigned char address, unsigned short value);

	unsigned int readRegister32b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
	void writeRegister32b(unsigned char address, unsigned int value);

	// for efficient bulk / low-level access:
	unsigned int getBufferSize();
	void readToAltBuffer(unsigned char * alt_read_buffer, unsigned int buffer_size);

	unsigned long int getUpdateCount() const;

	~Wallaby();

	static unsigned short getFirmwareVersion(unsigned char * alt_read_buffer = nullptr);

	static void atExit(bool should_abort);

private:
	Wallaby();
	bool transfer(unsigned char * alt_read_buffer = nullptr);
	void clearBuffers();
	unsigned char * write_buffer_;
	unsigned char * read_buffer_;
	const unsigned int buffer_size_; // same size for read/write buffers
	unsigned long int update_count_;
	int spi_fd_;
	std::mutex transfer_mutex_;

};

} /* namespace Private */

#endif /* SRC_WALLABY_P_HPP_ */

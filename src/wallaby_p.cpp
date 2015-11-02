/*
 * wallaby_p.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <cstring>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <string>
#include <iostream>


namespace Private
{

Wallaby::Wallaby()
: buffer_size_(REG_READABLE_COUNT),
  read_buffer_(new unsigned char[buffer_size_]),
  write_buffer_(new unsigned char[buffer_size_])
{
	static const std::string WALLABY_SPI_PATH = "/dev/spidev1.1";

	// TODO: move spi code outside constructor
	// TODO: handle device path better

    spi_fd_ = open(WALLABY_SPI_PATH.c_str(), O_RDWR);
    if (spi_fd_<=0)
    {
    		// TODO: ifndef guard std::cout calls
            std::cout << "Device not found: " << WALLABY_SPI_PATH << std::endl;
    }
}

Wallaby::~Wallaby()
{
	close(spi_fd_);
	delete[] write_buffer_;
	delete[] read_buffer_;
}

Wallaby * Wallaby::instance()
{
	static Wallaby instance;
	return &instance;
}

bool Wallaby::transfer(unsigned char * wbuf, unsigned char * rbuf, unsigned int len)
{
	if (spi_fd_ <= 0) return false; // TODO: feedback

	struct spi_ioc_transfer	xfer[1];
	memset(xfer, 0, sizeof xfer);
	xfer[0].tx_buf = (unsigned long) wbuf;
	xfer[0].rx_buf = (unsigned long) rbuf;
	xfer[0].len = REG_READABLE_COUNT;

	wbuf[0] = 'J';        //start
	wbuf[1] = 1;          // version 1
	wbuf[len-1] = 'S'; // stop

	std::cout << "xfer  start:" << std::to_string(wbuf[0]) << " ver:" << std::to_string(wbuf[1]) << " len:" << std::to_string(wbuf[2]) << " addy:" << std::to_string(wbuf[3]) << " val:" << std::to_string(wbuf[4]) << std::endl;

	int status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), xfer);
	usleep(2000); 	// TODO: remove this ...
					//it just makes sure we don't outrun the co-processor until interrupts are in place for DMA
	if (status < 0)
   	{
		std::cerr << "Error (SPI_IOC_MESSAGE): " << strerror(errno) << std::endl;
		return false;
	}

	return true;
}

bool Wallaby::readRegister(unsigned short address, unsigned char & value)
{
	if (address >= REG_ALL_COUNT) return false; // TODO: feedback

	clear_buffers();

	bool success = transfer(write_buffer_, read_buffer_, buffer_size_);

	if (success == false) return false;

	value = read_buffer_[address];

	return true;
}

bool Wallaby::writeRegister(unsigned short address, unsigned char value)
{
	if (address >= REG_ALL_COUNT) return false; // TODO: feedback

	clear_buffers();

	// TODO definitions for buffer inds
	write_buffer_[2] = 1; // write 1 register
	write_buffer_[3] = address; // at address 'address'
	write_buffer_[4] = value; // with value 'value'

	bool success = transfer(write_buffer_, read_buffer_, buffer_size_);

	return success;
}

void Wallaby::clear_buffers()
{
	memset(write_buffer_, 0, sizeof write_buffer_);
	memset(read_buffer_, 0, sizeof read_buffer_);
}

} /* namespace Private */

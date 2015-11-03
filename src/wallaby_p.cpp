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
  read_buffer_(new unsigned char[REG_READABLE_COUNT]),
  write_buffer_(new unsigned char[REG_READABLE_COUNT])
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

bool Wallaby::transfer()
{
	if (spi_fd_ <= 0) return false; // TODO: feedback

	write_buffer_[0] = 'J';        //start
	write_buffer_[1] = 1;          // version 1
	write_buffer_[buffer_size_-1] = 'S'; // stop

	struct spi_ioc_transfer	xfer[1];
	memset(xfer, 0, sizeof xfer);

	xfer[0].tx_buf = (unsigned long) write_buffer_;
	xfer[0].rx_buf = (unsigned long) read_buffer_;
	xfer[0].len = buffer_size_;

	int status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), xfer);
	usleep(100000); // TODO: remove this ...
					//it just makes sure we don't outrun the co-processor until interrupts are in place for DMA
	if (status < 0)
   	{
		std::cerr << "Error (SPI_IOC_MESSAGE): " << strerror(errno) << std::endl;
		return false;
	}

	return true;
}

unsigned char Wallaby::readRegister8b(unsigned char address)
{
	if (address >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	clear_buffers();

	//bool success = transfer();
	//TODO: if (success == false) return false;
	transfer();

	unsigned char value = read_buffer_[address];
	return value;
}

void Wallaby::writeRegister8b(unsigned char address, unsigned char value)
{
	if (address >= REG_ALL_COUNT) return;// false; // TODO: feedback

	clear_buffers();

	// TODO definitions for buffer inds
	write_buffer_[2] = 1; // write 1 register
	write_buffer_[3] = address; // at address 'address'
	write_buffer_[4] = value; // with value 'value'

	//TODO: bool success = transfer();
	//return success;
	transfer();
}

unsigned short Wallaby::readRegister16b(unsigned char address)
{
	if (address >= REG_READABLE_COUNT || address+1 >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	clear_buffers();

	//TODO: bool success = transfer();
	//return success;
	transfer();

	unsigned short value = (static_cast<unsigned short>(read_buffer_[address]) << 8) | read_buffer_[address+1];
	return value;
}

void Wallaby::writeRegister16b(unsigned char address, unsigned short value)
{
	if (address >= REG_ALL_COUNT || address+1 >= REG_ALL_COUNT) return;// false; // TODO: feedback

	clear_buffers();

	// TODO definitions for buffer inds
	write_buffer_[2] = 2; // write 2 registers
	write_buffer_[3] = address; // at address 'address'
	write_buffer_[4] = static_cast<unsigned char>((value & 0xFF00) >> 8);
	write_buffer_[5] = address + 1;
	write_buffer_[6] = static_cast<unsigned char>(value & 0x00FF);

	//TODO: bool success = transfer();
	//return success;
	transfer();
}

unsigned int Wallaby::readRegister32b(unsigned char address)
{
	if (address >= REG_READABLE_COUNT || address+3 >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	clear_buffers();

	//TODO: bool success = transfer();
	//return success;
	transfer();

	unsigned short value =
			  (static_cast<unsigned int>(read_buffer_[address]) << 24)
			| (static_cast<unsigned int>(read_buffer_[address+1]) << 16)
			| (static_cast<unsigned int>(read_buffer_[address+2]) << 8)
			| (static_cast<unsigned int>(read_buffer_[address+3]));

	return value;
}

void Wallaby::writeRegister32b(unsigned char address, unsigned int value)
{
	if (address >= REG_ALL_COUNT || address+3 >= REG_ALL_COUNT) return;// false; // TODO: feedback

	clear_buffers();

	// TODO definitions for buffer inds
	write_buffer_[2] = 2; // write 2 registers
	write_buffer_[3] = address; // at address 'address'
	write_buffer_[4] = static_cast<unsigned char>((value & 0xFF000000) >> 24);
	write_buffer_[5] = address + 1;
	write_buffer_[6] = static_cast<unsigned char>((value & 0x00FF0000) >> 16);
	write_buffer_[7] = address + 2;
	write_buffer_[8] = static_cast<unsigned char>((value & 0x0000FF00) >> 8);
	write_buffer_[9] = address + 3;
	write_buffer_[10] = static_cast<unsigned char>((value & 0x000000FF));

	//TODO: bool success = transfer();
	//return success;
	transfer();
}

void Wallaby::clear_buffers()
{
	memset(write_buffer_, 0, buffer_size_);
	memset(read_buffer_, 0, buffer_size_);
}

} /* namespace Private */

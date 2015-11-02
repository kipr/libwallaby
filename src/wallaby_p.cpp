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
}

Wallaby * Wallaby::instance()
{
	static Wallaby instance;
	return &instance;
}

bool Wallaby::readRegister(unsigned short address, unsigned char & value) const
{
    static const unsigned int BUFF_SIZE = REG_READABLE_COUNT; // TODO: cleanup

	if (spi_fd_ <=0) return false; // TODO: feedback

	if (address >= REG_ALL_COUNT) return false; // TODO: feedback

    unsigned char wbuf[BUFF_SIZE];
    unsigned char rbuf[BUFF_SIZE];

	memset(wbuf, 0, sizeof rbuf);
	memset(rbuf, 0, sizeof wbuf);

	struct spi_ioc_transfer	xfer[1];
	memset(xfer, 0, sizeof xfer);
	xfer[0].tx_buf = (unsigned long) wbuf;
	xfer[0].rx_buf = (unsigned long) rbuf;
	xfer[0].len = REG_READABLE_COUNT;

	wbuf[0] = 'J';        //start
	wbuf[1] = 1;          // version 1
	wbuf[2] = 0;          // 0 registers
	//wbuf[3] = 3;          // address
	//wbuf[4] = 0;          // value led off
	wbuf[BUFF_SIZE-1] = 'S'; // stop


	int status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), xfer);
	if (status < 0)
   	{
		std::cerr << "Error (SPI_IOC_MESSAGE): " << strerror(errno) << std::endl;
		return false;
	}

	value = rbuf[address];

	return true;
}


} /* namespace Private */

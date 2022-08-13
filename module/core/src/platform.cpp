/*
 * Platform_p.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/core/cleanup.hpp"


#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <cstring>

#include <csignal>
#include <thread>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef EMSCRIPTEN
	#include <linux/spi/spidev.h>
	#ifndef NOT_A_ZOOBEE_Platform
		#define SPI_FILE_SYSTEM "/dev/spidev0.0"
	#else
		#define SPI_FILE_SYSTEM "/dev/spidev2.0"
	#endif
#else
	#define SPI_FILE_SYSTEM "/dev/spidev0.0"
#endif

#include <string>
#include <iostream>
#include <iomanip> // std::hex

#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif


using namespace kipr;
using namespace kipr::core;

namespace
{
  void signal_handler(int s)
  {
    std::thread cleanup_thread(cleanup, true);
    cleanup_thread.detach();
  }
}


Platform::Platform()
: buffer_size_(REG_READABLE_COUNT),
  read_buffer_(new unsigned char[REG_READABLE_COUNT]),
  write_buffer_(new unsigned char[REG_READABLE_COUNT]),
  update_count_(0)
{
	static const std::string Platform_SPI_PATH = SPI_FILE_SYSTEM;

	// TODO: move spi code outside constructor
	// TODO: handle device path better

	spi_fd_ = open(Platform_SPI_PATH.c_str(), O_RDWR);
	if (spi_fd_<=0)
	{
		// TODO: ifndef guard std::cout calls
		std::cout << "Device not found: " << Platform_SPI_PATH << std::endl;
	}

	// register sig int handler
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = signal_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	sigaction(SIGTERM, &sigIntHandler, NULL);
}

Platform::~Platform()
{
	cleanup(false);

	close(spi_fd_);
	delete[] write_buffer_;
	delete[] read_buffer_;
}

Platform * Platform::instance()
{
	static Platform instance;
	return &instance;
}

bool Platform::transfer(unsigned char * alt_read_buffer)
{
	// Doesn't make sense for non-Platforms
#ifdef EMSCRIPTEN
	std::cerr << "Warning: this is not a Platform; transfer failed" << std::endl;
	return false;
#else
	if (spi_fd_ <= 0) return false; // TODO: feedback

	const unsigned char * const read_buffer = (alt_read_buffer == nullptr) ? read_buffer_ : alt_read_buffer;

	int status;

	// transfer counter - used to detect missed packets on co-proc side
	static unsigned char count = 0;
	count += 1;

	write_buffer_[0] = 'J';        //start
	write_buffer_[1] = WALLABY_SPI_VERSION;          // version #
	write_buffer_[2] = count;
	write_buffer_[buffer_size_-1] = 'S'; // stop

	struct spi_ioc_transfer	xfer[1];
	memset(xfer, 0, sizeof xfer);

	xfer[0].tx_buf = (unsigned long) write_buffer_;
	xfer[0].rx_buf = (unsigned long) read_buffer;
	xfer[0].len = buffer_size_;
	xfer[0].speed_hz = 16000000; // Josh: under /got on RPi...needed for Platform1?

	status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), xfer);
	update_count_ += 1;

	usleep(50); //FIXME: this  makes sure we don't outrun the co-processor until interrupts are in place for DMA

	if (status < 0)
	{
		std::cerr << "Error (SPI_IOC_MESSAGE): " << strerror(errno) << std::endl;
		return false;
	}

	if (read_buffer[0] != 'J')
	{
		std::cerr << " Error: DMA de-synchronized" << std::endl;

		for (unsigned int i = 0; i < buffer_size_; ++i)
		{
			std::cerr << std::hex << static_cast<unsigned int>(read_buffer[i]) << " ";
		}
		std::cerr << std::endl;

		return false;
	}

	return true;
#endif
}

#ifdef EMSCRIPTEN

// emscripten function to write register values to JS registers variable
EM_JS(void, updateRegisters, (unsigned char *addresses, unsigned char *values, int length), {
	const registers = [];
	for (let regIndex = 0; regIndex < length; regIndex++) {
		// Array values must be accessed directly from emscripten's memory buffer
		const address = HEAPU8[addresses + regIndex];
		const value = HEAPU8[values + regIndex];
		if(Module.context.registers[address] !== value) {
			Module.context.registers[address] = value;
			registers.push({ address: address, value: value });
		}
	}

	Module.context.onRegistersChange(registers);
});

// emscripten function to read register values from JS registers variable
EM_JS(unsigned char, readRegister, (unsigned char address), {
	return Module.context.registers[address];
});

#endif

unsigned char Platform::readRegister8b(unsigned char address, const unsigned char * alt_read_buffer)
{
	if (address >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	const unsigned char * const buffer = (alt_read_buffer == nullptr) ? read_buffer_ : alt_read_buffer;

	if (alt_read_buffer == nullptr)
	{
		#ifdef EMSCRIPTEN
		emscripten_sleep(0);
		read_buffer_[address] = readRegister(address);
		#else
		clearBuffers();

		//bool success = transfer();
		//TODO: if (success == false) return false;
		transfer();
		#endif
	}

	unsigned char value = buffer[address];
	return value;
}

void Platform::writeRegister8b(unsigned char address, unsigned char value)
{
	if (address >= REG_ALL_COUNT) return;// false; // TODO: feedback

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	#ifdef EMSCRIPTEN
	emscripten_sleep(0);
	unsigned char addresses[] = {address};
	unsigned char values[] = {value};
	updateRegisters(addresses, values, 1);
	#else
	clearBuffers();

	// TODO definitions for buffer inds
	write_buffer_[3] = 1; // write 1 register
	write_buffer_[4] = address; // at address 'address'
	write_buffer_[5] = value; // with value 'value'

	//TODO: bool success = transfer();
	//return success;
	transfer();
	#endif
}

unsigned short Platform::readRegister16b(unsigned char address, const unsigned char * alt_read_buffer)
{
	if (address >= REG_READABLE_COUNT || address+1 >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	const unsigned char * const buffer = (alt_read_buffer == nullptr) ? read_buffer_ : alt_read_buffer;

	if (alt_read_buffer == nullptr)
	{
		#ifdef EMSCRIPTEN
		emscripten_sleep(0);
		read_buffer_[address] = readRegister(address);
		read_buffer_[address+1] = readRegister(address+1);
		#else
		clearBuffers();

		//TODO: bool success = transfer();
		//return success;
		transfer();
		#endif
	}

	unsigned short value = (static_cast<unsigned short>(buffer[address]) << 8) | buffer[address+1];
	return value;
}

void Platform::writeRegister16b(unsigned char address, unsigned short value)
{
	if (address >= REG_ALL_COUNT || address+1 >= REG_ALL_COUNT) return;// false; // TODO: feedback

	unsigned char value1 = static_cast<unsigned char>((value & 0xFF00) >> 8);
	unsigned char value2 = static_cast<unsigned char>(value & 0x00FF);

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	#ifdef EMSCRIPTEN
	emscripten_sleep(0);
	unsigned char addresses[] = {address, static_cast<unsigned char>(address + 1)};
	unsigned char values[] = {value1, value2};
	updateRegisters(addresses, values, 2);
	#else
	clearBuffers();

	// TODO definitions for buffer inds
	write_buffer_[3] = 2; // write 2 registers
	write_buffer_[4] = address; // at address 'address'
	write_buffer_[5] = value1;
	write_buffer_[6] = address + 1;
	write_buffer_[7] = value2;

	//TODO: bool success = transfer();
	//return success;
	transfer();
	#endif
}

unsigned int Platform::readRegister32b(unsigned char address, const unsigned char * alt_read_buffer)
{
	if (address >= REG_READABLE_COUNT || address+3 >= REG_READABLE_COUNT) return 0;// false; // TODO: feedback

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	const unsigned char * const buffer = (alt_read_buffer == nullptr) ? read_buffer_ : alt_read_buffer;

	if (alt_read_buffer == nullptr)
	{
		#ifdef EMSCRIPTEN
		emscripten_sleep(0);
		read_buffer_[address] = readRegister(address);
		read_buffer_[address+1] = readRegister(address+1);
		read_buffer_[address+2] = readRegister(address+2);
		read_buffer_[address+3] = readRegister(address+3);
		#else
		clearBuffers();

		//TODO: bool success = transfer();
		//return success;
		transfer();
		#endif		
	}

	unsigned int value =
			(static_cast<unsigned int>(buffer[address]) << 24)
			| (static_cast<unsigned int>(buffer[address+1]) << 16)
			| (static_cast<unsigned int>(buffer[address+2]) << 8)
			| (static_cast<unsigned int>(buffer[address+3]));

	return value;
}

void Platform::writeRegister32b(unsigned char address, unsigned int value)
{
	if (address >= REG_ALL_COUNT || address+3 >= REG_ALL_COUNT) return;// false; // TODO: feedback

	unsigned char value1 = static_cast<unsigned char>((value & 0xFF000000) >> 24);
	unsigned char value2 = static_cast<unsigned char>((value & 0x00FF0000) >> 16);
	unsigned char value3 = static_cast<unsigned char>((value & 0x0000FF00) >> 8);
	unsigned char value4 = static_cast<unsigned char>((value & 0x000000FF));

	std::lock_guard<std::mutex> lock(transfer_mutex_);

	#ifdef EMSCRIPTEN
	emscripten_sleep(0);
	unsigned char addresses[] = {address, static_cast<unsigned char>(address + 1), static_cast<unsigned char>(address + 2), static_cast<unsigned char>(address + 3)};
	unsigned char values[] = {value1, value2, value3, value4};
	updateRegisters(addresses, values, 4);
	#else
	clearBuffers();

	// TODO definitions for buffer inds
	write_buffer_[3] = 4; // write 4 registers
	write_buffer_[4] = address; // at address 'address'
	write_buffer_[5] = value1;
	write_buffer_[6] = address + 1;
	write_buffer_[7] = value2;
	write_buffer_[8] = address + 2;
	write_buffer_[9] = value3;
	write_buffer_[10] = address + 3;
	write_buffer_[11] = value4;

	//TODO: bool success = transfer();
	//return succes2;
	transfer();
	#endif
}

void Platform::clearBuffers()
{
	memset(write_buffer_, 0, buffer_size_);
	memset(read_buffer_, 0, buffer_size_);
}


unsigned int Platform::getBufferSize()
{
	return buffer_size_;
}

void Platform::readToAltBuffer(unsigned char * alt_read_buffer, unsigned int buffer_size)
{
	if (buffer_size < buffer_size_)
	{
		std::cerr << "Error: readToAltBuffer has size" << buffer_size << " and needed to be at least " << buffer_size_ << std::endl;
		return;
	}

	clearBuffers();

	transfer(alt_read_buffer);
}

unsigned long int Platform::getUpdateCount() const
{
	return update_count_;
}

unsigned short Platform::getFirmwareVersion(unsigned char * alt_read_buffer)
{
	return Platform::instance()->readRegister16b(REG_R_VERSION_H, alt_read_buffer);
}

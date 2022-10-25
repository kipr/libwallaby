#include "../../device.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

#include "kipr/log/log.hpp"

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <mutex>

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SPI_FILE_SYSTEM ("/dev/spidev0.0")

namespace
{
  const std::string NAME = "wombat";
  kipr::log::Log logger("core/wombat");
}

class WombatDevice : public kipr::core::Device
{
public:
  WombatDevice()
    : spi_fd_(-1)
  {
    spi_fd_ = open(SPI_FILE_SYSTEM, O_RDWR);
    if (spi_fd_ <= 0)
    {
      logger.fatal() << "Not found: " << SPI_FILE_SYSTEM;
    }
  }

  virtual ~WombatDevice()
  {
    close(spi_fd_);
  }

  virtual const std::string &getName() const override
  {
    return NAME;
  }

  virtual std::uint8_t r8(const std::uint8_t address) override
  {
    std::uint8_t read_buffer[REG_ALL_COUNT];

    transfer(nullptr, read_buffer, sizeof (read_buffer));

    return read_buffer[address];
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    std::uint8_t read_buffer[REG_ALL_COUNT];

    transfer(nullptr, read_buffer, sizeof (read_buffer));

    return (
      read_buffer[address] << 8 |
      read_buffer[address + 1] << 0
    );
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    std::uint8_t read_buffer[REG_ALL_COUNT];

    transfer(nullptr, read_buffer, sizeof (read_buffer));

    return (
      read_buffer[address] << 24 |
      read_buffer[address + 1] << 16 |
      read_buffer[address + 2] << 8 |
      read_buffer[address + 3] << 0
    );
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    const std::uint8_t write_buffer[7] = {
      'J',
      WALLABY_SPI_VERSION,
      1,
      1,
      address,
      value,
      'S'
    };

    transfer(write_buffer, nullptr, sizeof (write_buffer));
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    const std::uint8_t write_buffer[9] = {
      'J',
      WALLABY_SPI_VERSION,
      1,
      2,
      address,
      (value & 0xFF00) >> 8,
      address + 1,
      (value & 0x00FF) >> 0,
      'S'
    };

    transfer(write_buffer, nullptr, sizeof (write_buffer));
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    const std::uint8_t write_buffer[13] = {
      'J',
      WALLABY_SPI_VERSION,
      1,
      4,
      address,
      (value & 0xFF000000) >> 24,
      address + 1,
      (value & 0x00FF0000) >> 16,
      address + 2,
      (value & 0x0000FF00) >> 8,
      address + 3,
      (value & 0x000000FF) >> 0,
      'S'
    };

    transfer(write_buffer, nullptr, sizeof (write_buffer));
  }

private:
  bool transfer(
    const std::uint8_t *const write_buffer,
    std::uint8_t *const read_buffer,
    const std::size_t size
  )
  {
    std::lock_guard<std::mutex> lock(mut_);

    struct spi_ioc_transfer xfer;
    memset(&xfer, 0, sizeof xfer);

    xfer.tx_buf = (unsigned long)write_buffer;
    xfer.rx_buf = (unsigned long)read_buffer;
    xfer.len = size;
    xfer.speed_hz = 16000000;

    const int status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), &xfer);

    usleep(50); // FIXME: this  makes sure we don't outrun the co-processor until interrupts are in place for DMA

    if (status < 0)
    {
      logger.error() << "SPI_IOC_MESSAGE: " << strerror(errno);
      return false;
    }

    if (read_buffer && read_buffer[0] != 'J')
    {
      logger.error() << "DMA de-synchronized";
      return false;
    }

    return true;
  }

  int spi_fd_;
  std::mutex mut_;
};

struct WombatDeviceDescriptor
{
  typedef WombatDevice DeviceType;

  static bool isPresent()
  {
    return access(SPI_FILE_SYSTEM, F_OK) == 0;
  }
};

KIPR_CORE_PLATFORM_DEVICE_REGISTER(WombatDeviceDescriptor);

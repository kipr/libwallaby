#include "../../device.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

#include "kipr/log/log.hpp"

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <mutex>
#include <chrono>

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mutex"

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
      : spi_fd_(-1),
        count(0),
        rejected_seen_(false),
        rejected_last_(0),
        rejected_pending_(0),
        read_buf(new std::uint8_t[REG_READABLE_COUNT]),
        write_buf(new std::uint8_t[REG_READABLE_COUNT])
  {
    spi_fd_ = open(SPI_FILE_SYSTEM, O_RDWR);
    if (spi_fd_ <= 0)
    {
      logger.fatal() << "Not found: " << SPI_FILE_SYSTEM;
    }
    clear_buffers();
  }

  virtual ~WombatDevice()
  {
    std::cout << "~Wombat()" << std::endl;
    if (rejected_pending_)
      logger.warning() << "co-processor rejected " << rejected_pending_
                       << " corrupt SPI packet(s); some writes may have been lost";
    close(spi_fd_);
    delete[] read_buf;
    delete[] write_buf;
  }

  virtual const std::string &getName() const override
  {
    return NAME;
  }

  virtual std::uint8_t r8(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    transfer();

    return read_buf[address];
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    transfer();

    return (
        read_buf[address] << 8 |
        read_buf[address + 1] << 0);
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    transfer();

    return (
        read_buf[address] << 24 |
        read_buf[address + 1] << 16 |
        read_buf[address + 2] << 8 |
        read_buf[address + 3] << 0);
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    write_buf[3] = 1,
    write_buf[4] = address,
    write_buf[5] = value;

    transfer();
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    write_buf[3] = 2,
    write_buf[4] = address,
    write_buf[5] = (value & 0xFF00) >> 8,
    write_buf[6] = address + 1,
    write_buf[7] = (value & 0x00FF) >> 0,

    transfer();
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    clear_buffers();
    write_buf[3] = 4;
    write_buf[4] = address,
    write_buf[5] = (value & 0xFF000000) >> 24,
    write_buf[6] = address + 1,
    write_buf[7] = (value & 0x00FF0000) >> 16,
    write_buf[8] = address + 2,
    write_buf[9] = (value & 0x0000FF00) >> 8,
    write_buf[10] = address + 3,
    write_buf[11] = (value & 0x000000FF) >> 0,

    transfer();
  }

private:
  void clear_buffers()
  {
    memset(write_buf, 0, REG_READABLE_COUNT);
    memset(read_buf, 0, REG_READABLE_COUNT);
  }

  bool transfer()
  {
    ++count;
    write_buf[0] = 'J';
    write_buf[1] = WALLABY_SPI_VERSION;
    write_buf[2] = count;
    write_buf[REG_READABLE_COUNT - 1] = 'S';

    struct spi_ioc_transfer xfer[1];
    memset(xfer, 0, sizeof xfer);

    xfer[0].tx_buf = (unsigned long)write_buf;
    xfer[0].rx_buf = (unsigned long)read_buf;
    xfer[0].len = REG_READABLE_COUNT;
    xfer[0].speed_hz = 16000000;

    const int status = ioctl(spi_fd_, SPI_IOC_MESSAGE(1), xfer);

    usleep(50); // FIXME: this  makes sure we don't outrun the co-processor until interrupts are in place for DMA

    if (status < 0)
    {
      logger.error() << "SPI_IOC_MESSAGE: " << strerror(errno);
      return false;
    }

    if (read_buf[0] != static_cast<unsigned char>('J'))
    {
      logger.error() << "DMA de-synchronized";
      return false;
    }

    check_rejected();
    return true;
  }

  // The firmware counts packets it had to repair or drop writes from (corrupt
  // count byte, out-of-range address) in the high nibble of REG_RW_MOT_DONE,
  // mod 16. Older firmware leaves it 0. Warn when it moves, at most once every
  // few seconds, so a bad link shows up without flooding the output.
  void check_rejected()
  {
    const std::uint8_t now = read_buf[REG_RW_MOT_DONE] >> 4;
    if (!rejected_seen_)
    {
      rejected_seen_ = true;
      rejected_last_ = now;
      return;
    }

    const std::uint8_t step = (now - rejected_last_) & 0x0F;
    // A step of -1 is the firmware's main loop briefly writing back the
    // previous count; it is republished on the next packet.
    if (step != 0 && step != 0x0F)
    {
      rejected_last_ = now;
      rejected_pending_ += step;
    }
    if (rejected_pending_ == 0) return;

    const auto t = std::chrono::steady_clock::now();
    if (rejected_warned_ != std::chrono::steady_clock::time_point() && t - rejected_warned_ < std::chrono::seconds(5)) return;
    logger.warning() << "co-processor rejected " << rejected_pending_
                     << " corrupt SPI packet(s); some writes may have been lost";
    rejected_pending_ = 0;
    rejected_warned_ = t;
  }

  int spi_fd_;
  std::mutex mut_;
  std::uint8_t count;
  bool rejected_seen_;
  std::uint8_t rejected_last_;
  unsigned rejected_pending_;
  std::chrono::steady_clock::time_point rejected_warned_;
  std::uint8_t *write_buf;
  std::uint8_t *read_buf;
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

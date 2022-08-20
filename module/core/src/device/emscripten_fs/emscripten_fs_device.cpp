#include "../../device.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/log/log.hpp"

#include <cstdint>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

namespace
{
  const std::string NAME = "emscripten_fs";
  const char *const DEVICE_PATH = "/registers";
  kipr::log::Log logger("core/emscripten_fs"); 

  const std::uint8_t REQUEST_READ = 0;
  const std::uint8_t REQUEST_WRITE = 1;
}

class EmscriptenFsDevice : public kipr::core::Device
{
public:
  EmscriptenFsDevice()
    : fd_(-1)
    , write_buffer_(nullptr)
    , write_buffer_size_(0)
  {
    fd_ = open(DEVICE_PATH, O_RDWR);
    if (fd_ < 0)
    {
      logger.fatal() << "Failed to open \"" << DEVICE_PATH << "\"";
    }
  }

  virtual ~EmscriptenFsDevice()
  {
    delete[] write_buffer_;
  }

  virtual const std::string &getName() const override
  {
    return NAME;
  }

  virtual std::uint8_t r8(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    
    std::uint8_t value;
    if (!read(address, &value, 1))
    {
      logger.error() << "r8 request failed";
      return 0;
    }

    return value;
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);
    
    std::uint8_t values[2];
    if (!read(address, values, 2))
    {
      logger.error() << "r16 request failed";
      return 0;
    }

    return (
      values[0] << 8 |
      values[1] << 0
    );
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    std::lock_guard<std::mutex> lock(mut_);

    std::uint8_t values[4];
    if (!read(address, values, 4))
    {
      logger.error() << "r32 request failed";
      return 0;
    }

    return (
      values[0] << 24 |
      values[1] << 16 |
      values[2] << 8 |
      values[3] << 0
    );
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    write(address, &value, 1);
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    const std::uint8_t values[2] = {
      static_cast<std::uint8_t>((value & 0xFF00) >> 8),
      static_cast<std::uint8_t>(value & 0x00FF)
    };

    write(address, values, 2);
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    const std::uint8_t values[4] = {
      static_cast<std::uint8_t>((value & 0xFF000000) >> 24),
      static_cast<std::uint8_t>((value & 0x00FF0000) >> 16),
      static_cast<std::uint8_t>((value & 0x0000FF00) >> 8),
      static_cast<std::uint8_t>(value & 0x000000FF)
    };

    write(address, values, 4);
  }

private:
  bool read(const std::uint8_t address, std::uint8_t *const values, const std::uint8_t count)
  {
    const std::uint8_t buffer[3] = { REQUEST_READ, address, count };
    
    ssize_t res = ::write(fd_, buffer, sizeof (buffer));
    if (res != sizeof (buffer))
    {
      logger.error() << "(read) write failed with result: " << res;
      return false;
    }

    res = ::read(fd_, values, count);
    if (res != count)
    {
      logger.error() << "(read) read failed with: " << res;
      return false;
    }

    return true;
  }

  bool write(const std::uint8_t address, const std::uint8_t *const values, const std::uint8_t count)
  {
    const std::uint8_t total_size = 3 + count;

    if (!write_buffer_)
    {
      write_buffer_size_ = total_size;
      write_buffer_ = new std::uint8_t[write_buffer_size_];
    }
    else
    {
      if (write_buffer_size_ < total_size)
      {
        delete[] write_buffer_;
        write_buffer_size_ = total_size;
        write_buffer_ = new std::uint8_t[write_buffer_size_];
      }
    }

    write_buffer_[0] = REQUEST_WRITE;
    write_buffer_[1] = address;
    write_buffer_[2] = count;
    memcpy(write_buffer_ + 3, values, count);

    const ssize_t res = ::write(fd_, write_buffer_, total_size);
    if (res != total_size)
    {
      logger.error() << "(write) write failed with result: " << res;
      return false;
    }

    return true;
  }

  int fd_;
  std::mutex mut_;

  std::uint8_t *write_buffer_;
  std::uint8_t write_buffer_size_;
};

struct EmscriptenFsDeviceDescriptor
{
  typedef EmscriptenFsDevice DeviceType;

  static bool isPresent()
  {
    return true;
    // return access(DEVICE_PATH, F_OK) == 0;
  }
};

KIPR_CORE_PLATFORM_DEVICE_REGISTER(EmscriptenFsDeviceDescriptor);

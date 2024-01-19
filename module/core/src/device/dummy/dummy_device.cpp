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
  const std::string NAME = "dummy";
  kipr::log::Log logger("core/dummy"); 
}

class DummyDevice : public kipr::core::Device
{
public:
  DummyDevice()
  {
  }

  virtual ~DummyDevice()
  {
  }

  virtual const std::string &getName() const override
  {
    return NAME;
  }

  virtual std::uint8_t r8(const std::uint8_t address) override
  {
    logger.info() << "r8(" << (int)address << ")";
    return 0;
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    logger.info() << "r16(" << (int)address << ")";
    return 0;
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    logger.info() << "r32(" << (int)address << ")";
    return 0;
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    logger.info() << "w8(" << (int)address << ", " << (int)value << ")";
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    logger.info() << "w16(" << (int)address << ", " << value << ")";
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    logger.info() << "w32(" << (int)address << ", " << value << ")";
  }
};

struct DummyDeviceDescriptor
{
  typedef DummyDevice DeviceType;

  static bool isPresent()
  {
    return true;
  }
};

KIPR_CORE_PLATFORM_DEVICE_REGISTER(DummyDeviceDescriptor);

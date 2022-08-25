#ifndef _KIPR_CORE_DEVICE_HPP_
#define _KIPR_CORE_DEVICE_HPP_

#include <cstdint>
#include <string>

namespace kipr
{
  namespace core
  {
    struct Command;

    class Device
    {
    public:
      virtual ~Device();

      virtual const std::string &getName() const = 0;
      
      virtual std::uint8_t r8(const std::uint8_t address) = 0;
      virtual std::uint16_t r16(const std::uint8_t address) = 0;
      virtual std::uint32_t r32(const std::uint8_t address) = 0;

      virtual void w8(const std::uint8_t address, const std::uint8_t value) = 0;
      virtual void w16(const std::uint8_t address, const std::uint16_t value) = 0;
      virtual void w32(const std::uint8_t address, const std::uint32_t value) = 0;

      virtual void submit(const Command *const buffer, const std::size_t size);
    };
  }
}

#endif

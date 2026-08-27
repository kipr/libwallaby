#include "device.hpp"

#include "kipr/core/command.hpp"

using namespace kipr;
using namespace kipr::core;

Device::~Device()
{
}

// Naive implementation of `submit()` to ease transition.
void Device::submit(const Command *const buffer, const std::size_t size)
{
  for (std::size_t i = 0; i < size; ++i)
  {
    const Command &command = buffer[i];

    const std::uint8_t address = command.address;
    const std::uint32_t mask = command.mask;
    const std::uint8_t size = command.size;
    
    switch (command.type)
    {
      case Command::Type::Read:
      {
        switch (size)
        {
          case 1:
          {
            *command.value &= ~mask;
            *command.value |= r8(address);
            break;
          }

          case 2:
          {
            std::uint16_t &value = *reinterpret_cast<std::uint16_t *>(command.value);
            value &= ~mask;
            value |= r16(address);
            break;
          }
          case 4:
          {
            std::uint32_t &value = *reinterpret_cast<std::uint32_t *>(command.value);
            value &= ~mask;
            value |= r32(address);
            break;
          }
        }
        break;
      }
      case Command::Type::Write:
      {
        switch (size)
        {
          case 1:
          {
            if (mask != 0xFF)
            {
              std::uint8_t value = r8(address);
              value &= ~mask;
              value |= *command.value & mask;
              w8(address, value);
            }
            else
            {
              w8(address, *command.value);
            }
            break;
          }

          case 2:
          {
            if (mask != 0xFFFF)
            {
              std::uint16_t value = r16(address);
              value &= ~mask;
              value |= *reinterpret_cast<const uint16_t *>(command.value) & mask;
              w16(address, value);
            }
            else
            {
              w16(address, *reinterpret_cast<const uint16_t *>(command.value));
            }
            break;
          }
          case 4:
          {
            if (mask != 0xFFFFFFFF)
            {
              std::uint32_t value = r32(address);
              value &= ~mask;
              value |= *reinterpret_cast<const uint32_t *>(command.value) & mask;
              w32(address, value);
            }
            else
            {
              w32(address, *reinterpret_cast<const uint32_t *>(command.value));
            }
            break;
          }
        }
        break;
      }
      case Command::Type::Fence:
      {
        break;
      }
    }
  }
}
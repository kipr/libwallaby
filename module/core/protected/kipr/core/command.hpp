#ifndef _KIPR_CORE_COMMAND_HPP_
#define _KIPR_CORE_COMMAND_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include <array>

namespace kipr
{
  namespace core
  {
    struct Command;

    struct Command
    {
      enum class Type : std::uint8_t
      {
        Fence,
        Read,
        Write
      };

      Type type;
      std::uint8_t address;
      std::uint8_t size;
      std::uint8_t *value;

      // If mask isn't 0xFF... (for the size in question),
      // then an implicit fence is added before the current fence executes.
      uint32_t mask;
    };

    extern const Command FENCE;

    inline Command r8(const std::uint8_t address, std::uint8_t *const value, const uint8_t mask = 0xFF)
    {
      Command command;
      command.type = Command::Type::Read;
      command.address = address;
      command.size = sizeof(std::uint8_t);
      command.value = value;
      command.mask = mask;
      return command;
    }

    inline Command r16(const std::uint8_t address, std::uint16_t *const value, const uint16_t mask = 0xFFFF)
    {
      Command command;
      command.type = Command::Type::Read;
      command.address = address;
      command.size = sizeof(std::uint16_t);
      command.value = reinterpret_cast<std::uint8_t *>(value);
      command.mask = mask;
      return command;
    }

    inline Command r32(const std::uint8_t address, std::uint32_t *const value, const uint32_t mask = 0xFFFFFFFF)
    {
      Command command;
      command.type = Command::Type::Read;
      command.address = address;
      command.size = sizeof(std::uint32_t);
      command.value = reinterpret_cast<std::uint8_t *>(value);
      command.mask = mask;
      return command;
    }

    inline Command w8(const std::uint8_t address, const std::uint8_t *const value, const uint8_t mask = 0xFF)
    {
      Command command;
      command.type = Command::Type::Write;
      command.address = address;
      command.size = sizeof(std::uint8_t);
      // Safety: value is guaranteed to not be mutated.
      command.value = const_cast<std::uint8_t *>(value);
      command.mask = mask;
      return command;
    }

    inline Command w16(const std::uint8_t address, const std::uint16_t *const value, const uint16_t mask = 0xFFFF)
    {
      Command command;
      command.type = Command::Type::Write;
      command.address = address;
      command.size = sizeof(std::uint16_t);
      // Safety: value is guaranteed to not be mutated.
      command.value = const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(value));
      command.mask = mask;
      return command;
    }

    inline Command w32(const std::uint8_t address, const std::uint32_t *const value, const uint32_t mask = 0xFFFFFFFF)
    {
      Command command;
      command.type = Command::Type::Write;
      command.address = address;
      command.size = sizeof(std::uint32_t);
      // Safety: value is guaranteed to not be mutated.
      command.value = const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(value));
      command.mask = mask;
      return command;
    }

    struct Memory
    {
    public:
      template<typename T>
      struct Chunk
      {
        Chunk()
          : offset(0)
        {
        }

        std::array<T, 16> data;
        std::size_t offset;
      };

      inline std::uint8_t *u8(const std::uint8_t value = 0)
      {
        return next(u8s_, value);
      }

      inline std::uint16_t *u16(const std::uint16_t value = 0)
      {
        return next(u16s_, value);
      }

      inline std::uint32_t *u32(const std::uint32_t value = 0)
      {
        return next(u32s_, value);
      }

    private:
      template<typename T>
      T *next(std::vector<std::unique_ptr<Chunk<T>>> &vec, const T value = T())
      {
        if (vec.empty()) vec.emplace_back();
        
        Chunk<T> *last = &*vec.back();
        if (last->offset == last->data.size())
        {
          vec.emplace_back();
          last = &*vec.back();
        }

        T &ret = last->data[last->offset++];
        ret = value;
        return &ret;
      }

      std::vector<std::unique_ptr<Chunk<std::uint8_t>>> u8s_;
      std::vector<std::unique_ptr<Chunk<std::uint16_t>>> u16s_;
      std::vector<std::unique_ptr<Chunk<std::uint32_t>>> u32s_;
    };
  }
}

#endif
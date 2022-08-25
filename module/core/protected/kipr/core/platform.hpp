#ifndef _KIPR_CORE_PLATFORM_HPP_
#define _KIPR_CORE_PLATFORM_HPP_

#include <mutex>
#include <vector>
#include <iostream>
#include <memory>

#include "command.hpp"

namespace kipr
{
  namespace core
  {
    class Device;

    // See comment for KIPR_CORE_PLATFORM_DEVICE_REGISTER for information
    // on why this exists.
    extern std::unique_ptr<Device> DEVICE;

    class Platform
    {
    public:
      ~Platform();
      
      static Platform *instance();

      unsigned char readRegister8b(unsigned char address);
      void writeRegister8b(unsigned char address, unsigned char value);

      unsigned short readRegister16b(unsigned char address);
      void writeRegister16b(unsigned char address, unsigned short value);

      unsigned int readRegister32b(unsigned char address);
      void writeRegister32b(unsigned char address, unsigned int value);
      
      void submit(const Command *const buffer, const std::size_t size);

      template<typename... Args>
      void submit(Args &&...args)
      {
        std::vector<Command> commands;
        commands.reserve(submitSize(args...));
        buildSubmit(commands, args...);
        submit(command.cbegin(), command.cend());
      }

    private:
      Platform();

      template<typename... Args>
      void buildSubmit(std::vector<Command> &commands, Command &&first, Args &&...args)
      {
        commands.emplace_back(first);
        buildSubmit(commands, args...);
      }

      template<typename T, typename... Args>
      void buildSubmit(std::vector<Command> &commands, T &&first, Args &&...args)
      {
        commands.insert(commands.end(), first.cbegin(), first.cend());
        buildSubmit(commands, args...);
      }

      template<typename T>
      void buildSubmit(std::vector<Command> &commands, T &&first)
      {
        commands.insert(commands.end(), first.cbegin(), first.cend());
      }

      template<typename... Args>
      size_t submitSize(const Command &first, const Args &...args)
      {
        return 1 + submitSize(args...);
      }

      template<typename T, typename... Args>
      size_t submitSize(const T &first, const Args &...args)
      {
        return submitSize(first.size());
      }

      size_t submitSize(const Command &first)
      {
        return 1;
      }

      template<typename T>
      size_t submitSize(const T &first)
      {
        return first.size();
      }

      static std::mutex instance_mut_;
      static Platform *instance_;
    };
  }
}

// Typically we would create __attribute__((constructor))s
// and register each available Device with the Platform. There's
// a bug in emscripten's WASM dynamic linking, though, where
// __attribute__((constructor))s run before global data initialization,
// causing all sorts of bizarre bugs. Instead we'll set a global to
// the appropriate Device, since we know we'll only have one compiled
// into the executable for now. Sigh.
#define KIPR_CORE_PLATFORM_DEVICE_REGISTER(descriptor) \
  std::unique_ptr<kipr::core::Device> kipr::core::DEVICE(new descriptor::DeviceType());

#endif

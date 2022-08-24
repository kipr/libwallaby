#ifndef _KIPR_CORE_PLATFORM_HPP_
#define _KIPR_CORE_PLATFORM_HPP_

#include <mutex>
#include <vector>
#include <iostream>
#include <memory>

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

    private:
      Platform();

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

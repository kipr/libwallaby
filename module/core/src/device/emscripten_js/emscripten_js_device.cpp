#include "../../device.hpp"
#include "kipr/core/platform.hpp"

#include "emscripten.h"

namespace
{
  const std::string NAME = "emscripten_js";

  // emscripten functions to write register values to JS
  EM_JS(void, jsSetRegister8b, (unsigned char address, unsigned char value), {
    Module.context.setRegister8b(address, value);
  });

  EM_JS(void, jsSetRegister16b, (unsigned char address, unsigned short value), {
    Module.context.setRegister16b(address, value);
  });

  EM_JS(void, jsSetRegister32b, (unsigned char address, unsigned int value), {
    Module.context.setRegister32b(address, value);
  });

  EM_JS(unsigned char, jsReadRegister8b, (unsigned char address), {
    return Module.context.readRegister8b(address);
  });

  EM_JS(unsigned short, jsReadRegister16b, (unsigned char address), {
    return Module.context.readRegister16b(address);
  });

  EM_JS(unsigned int, jsReadRegister32b, (unsigned char address), {
    return Module.context.readRegister32b(address);
  });
}

class EmscriptenJsDevice : public kipr::core::Device
{
public:
  virtual ~EmscriptenJsDevice()
  {
  }

  virtual const std::string &getName() const override
  {
    return NAME;
  }

  virtual std::uint8_t r8(const std::uint8_t address) override
  {
    return jsReadRegister8b(address);
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    return jsReadRegister16b(address);
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    return jsReadRegister32b(address);
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    jsSetRegister8b(address, value);
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    jsSetRegister16b(address, value);
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    jsSetRegister32b(address, value);
  }
};

struct EmscriptenJsDeviceDescriptor
{
  typedef EmscriptenJsDevice DeviceType;

  static bool isPresent()
  {
    return true;
  }
};

KIPR_CORE_PLATFORM_DEVICE_REGISTER(EmscriptenJsDeviceDescriptor);

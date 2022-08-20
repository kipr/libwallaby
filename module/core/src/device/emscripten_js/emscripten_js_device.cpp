#include "../../device.hpp"
#include "kipr/core/platform.hpp"


#include "emscripten.h"

namespace
{
  const std::string NAME = "emscripten_js";

  // emscripten function to write register values to JS registers variable
  EM_JS(void, updateRegisters, (unsigned char *addresses, unsigned char *values, int length), {
    const registers = [];
    for (let regIndex = 0; regIndex < length; regIndex++) {
      // Array values must be accessed directly from emscripten's memory buffer
      const address = HEAPU8[addresses + regIndex];
      const value = HEAPU8[values + regIndex];
      if(Module.context.registers[address] !== value) {
        Module.context.registers[address] = value;
        registers.push({ address: address, value: value });
      }
    }

    Module.context.onRegistersChange(registers);
  });

  // emscripten function to read register values from JS registers variable
  EM_JS(unsigned char, readRegister, (unsigned char address), {
    return Module.context.registers[address];
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
    emscripten_sleep(0);
    return readRegister(address);
  }

  virtual std::uint16_t r16(const std::uint8_t address) override
  {
    emscripten_sleep(0);

    return (
      readRegister(address) << 8 |
      readRegister(address + 1)
    );
  }

  virtual std::uint32_t r32(const std::uint8_t address) override
  {
    emscripten_sleep(0);

    return (
      readRegister(address) << 24 |
      readRegister(address + 1) << 16 |
      readRegister(address + 2) << 8 |
      readRegister(address + 3)
    );
  }

  virtual void w8(const std::uint8_t address, const std::uint8_t value) override
  {
    emscripten_sleep(0);
    std::uint8_t addresses[] = { address };
    std::uint8_t values[] = { value };
    updateRegisters(addresses, values, 1);
  }

  virtual void w16(const std::uint8_t address, const std::uint16_t value) override
  {
    emscripten_sleep(0);
    
    std::uint8_t addresses[] = {
      address,
      static_cast<std::uint8_t>(address + 1)
    };

    std::uint8_t values[] = {
      static_cast<std::uint8_t>((value & 0xFF00) >> 8),
      static_cast<std::uint8_t>(value & 0x00FF)
    };

    updateRegisters(addresses, values, 2);
  }

  virtual void w32(const std::uint8_t address, const std::uint32_t value) override
  {
    emscripten_sleep(0);
    
    std::uint8_t addresses[] = {
      address,
      static_cast<std::uint8_t>(address + 1),
      static_cast<std::uint8_t>(address + 2),
      static_cast<std::uint8_t>(address + 3)
    };

    std::uint8_t values[] = {
      static_cast<std::uint8_t>((value & 0xFF000000) >> 24),
      static_cast<std::uint8_t>((value & 0x00FF0000) >> 16),
      static_cast<std::uint8_t>((value & 0x0000FF00) >> 8),
      static_cast<std::uint8_t>(value & 0x000000FF)
    };
    
    updateRegisters(addresses, values, 4);
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

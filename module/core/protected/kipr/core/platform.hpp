#ifndef _KIPR_CORE_PLATFORM_HPP_
#define _KIPR_CORE_PLATFORM_HPP_

#include <mutex>
#include <vector>

namespace kipr
{
  namespace core
  {
    class Device;

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

      template<typename T>
      static void registerDevice()
      {
        if (!T::isPresent()) return;
        std::shared_ptr<typename T::DeviceType> device(new typename T::DeviceType());
        Platform::instance()->devices_.push_back(device);
      }

    private:
      Platform(const std::shared_ptr<Device> &device);
      std::shared_ptr<Device> device_;

      static std::mutex instance_mut_;
      static Platform *instance_;

      static std::vector<std::shared_ptr<Device>> devices_;
    };
  }
}

#define KIPR_CORE_PLATFORM_DEVICE_REGISTER(descriptor) \
  __attribute__((constructor)) \
  static void descriptor##_register() \
  { \
    kipr::core::Platform::registerDevice<descriptor>(); \
  }

#endif

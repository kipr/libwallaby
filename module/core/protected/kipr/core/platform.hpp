#ifndef _KIPR_CORE_PLATFORM_HPP_
#define _KIPR_CORE_PLATFORM_HPP_

#include <mutex>

namespace kipr
{
  namespace core
  {
    class Platform
    {
    public:
      ~Platform();
      
      static Platform * instance();

      unsigned char readRegister8b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
      void writeRegister8b(unsigned char address, unsigned char value);

      unsigned short readRegister16b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
      void writeRegister16b(unsigned char address, unsigned short value);

      unsigned int readRegister32b(unsigned char address, const unsigned char * alt_read_buffer = nullptr);
      void writeRegister32b(unsigned char address, unsigned int value);

      // for efficient bulk / low-level access:
      unsigned int getBufferSize();
      void readToAltBuffer(unsigned char * alt_read_buffer, unsigned int buffer_size);

      unsigned long int getUpdateCount() const;

      static unsigned short getFirmwareVersion(unsigned char * alt_read_buffer = nullptr);

    private:
      Platform();
      bool transfer(unsigned char * alt_read_buffer = nullptr);
      void clearBuffers();
      unsigned char * write_buffer_;
      unsigned char * read_buffer_;
      const unsigned int buffer_size_; // same size for read/write buffers
      unsigned long int update_count_;
      int spi_fd_;
      std::mutex transfer_mutex_;

    };
  }
}

#endif

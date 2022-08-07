#include "magneto_p.hpp"

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

using namespace kipr;
using namespace kipr::magneto;

using kipr::core::Platform;

short kipr::magneto::magneto_x(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_MAG_X_H, alt_read_buffer))/16;
}

short kipr::magneto::magneto_y(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_MAG_Y_H, alt_read_buffer))/16;
}

short kipr::magneto::magneto_z(unsigned char * alt_read_buffer)
{
    return static_cast<signed short>(Platform::instance()->readRegister16b(REG_RW_MAG_Z_H, alt_read_buffer))/16;
}

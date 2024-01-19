#include "analog_p.hpp"

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

using namespace kipr;
using namespace kipr::analog;

using kipr::core::Platform;

unsigned short kipr::analog::analog_value(unsigned int port)
{
    // TODO port # checks, shared # of ports constant
    const unsigned char address = REG_RW_ADC_0_H + 2 * port;
    return Platform::instance()->readRegister16b(address);
}

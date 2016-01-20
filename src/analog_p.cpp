/*
 * analog_p.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "analog_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

namespace Private
{

unsigned short analog_value(unsigned int port, unsigned char * alt_read_buffer)
{
    // TODO port # checks, shared # of ports constant
    const unsigned char address = REG_RW_ADC_0_H + 2 * port;
    return Private::Wallaby::instance()->readRegister16b(address, alt_read_buffer);
}

}

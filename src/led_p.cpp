/*
 * led_p.cpp
 *
 *  Created on: Dec 30, 2015
 *      Author: Joshua Southerland
 */

#include "led_p.hpp"

#include <unistd.h> // TODO: remove/replaece for windows? led.cpp is going away though

#include "wallaby_regs_p.hpp"
#include "wallaby_p.hpp"

namespace Private
{

static const unsigned int LED_PIN_NUM = 12;

void config_led()
{
  unsigned short outputs = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OE_H);

  //  bit = 1 for output, 0 for input
  outputs |= (1 << LED_PIN_NUM);

  Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OE_H, outputs);

  usleep(1000);
}

// TODO: move/remove
void blink_led()
{

  // led (on?)
  unsigned short out;
  out = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OUT_H);
  out |= (1 << LED_PIN_NUM);
  Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OUT_H, out);

  usleep(20000);

  // led (off?)
  out = Private::Wallaby::instance()->readRegister16b(REG_RW_DIG_OUT_H);
  out &= ~(1 << LED_PIN_NUM);
  Private::Wallaby::instance()->writeRegister16b(REG_RW_DIG_OUT_H, out);
  usleep(20000);
}

}

/*
 * battery.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/battery.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

// TODO: some way to do SPI

bool Battery::isCharging()
{
	return false; // this board doesn't support charging
}


float Battery::powerLevel()
{
	unsigned char high_byte, low_byte;

	// get current voltage reading
	// TODO: read both values at the same time, once we support that
	Private::Wallaby::instance()->readRegister(REG_RW_BATT_H, high_byte);
	Private::Wallaby::instance()->readRegister(REG_RW_BATT_L, low_byte);

	// piece the 12-bit ADC result back together
	unsigned short raw_batt_adc = (static_cast<unsigned short>(high_byte) << 8) | static_cast<unsigned short>(low_byte);

	// calculate voltage based on linear curve-fitting
	float batt_voltage = -0.02070635f + 0.009071161f * static_cast<float>(raw_batt_adc);

	return batt_voltage;
}

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
	// piece the 12-bit ADC result back together
	unsigned short raw_batt_adc = Private::Wallaby::instance()->readRegister16b(REG_RW_BATT_H);

	// calculate voltage based on linear curve-fitting
	float batt_voltage = -0.02070635f + 0.009071161f * static_cast<float>(raw_batt_adc);

	// FIXME   ADC->capacity  or  voltage->capacity

	return batt_voltage;
}

/*
 * accel_p.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "accel_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

Accel::~Accel()
{

}

signed short Accel::xAccel() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_X_H));
	return val;
}

signed short Accel::yAccel() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Y_H));
	return val;
}

signed short Accel::zAccel() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_ACCEL_Z_H));
	return val;
}

Accel * Accel::instance()
{
	static Accel instance;
	return &instance;
}

bool Accel::calibrate() const
{
	return true; // success
}

Accel::Accel()
{

}

Accel::Accel(const Accel & rhs)
{

}

Accel & Accel::operator=(const Accel & rhs)
{
	return *this;
}

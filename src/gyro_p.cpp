/*
 * gyro_p.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */




#include "gyro_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

Gyro::~Gyro()
{

}

signed short Gyro::xGyro() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_X_H));
	return val;
}

signed short Gyro::yGyro() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Y_H));
	return val;
}

signed short Gyro::zGyro() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_GYRO_Z_H));
	return val;
}

Gyro * Gyro::instance()
{
	static Gyro instance;
	return &instance;
}

bool Gyro::calibrate() const
{
	return true; // success
}

Gyro::Gyro()
{

}

Gyro::Gyro(const Gyro & rhs)
{

}

Gyro & Gyro::operator=(const Gyro & rhs)
{
	return *this;
}

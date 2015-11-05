/*
 * magneto_p.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_MAGNETO_P_CPP_
#define SRC_MAGNETO_P_CPP_



#include "magneto_p.hpp"

#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

using namespace Private;

Magneto::~Magneto()
{

}

signed short Magneto::xMagneto() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_X_H));
	return val;
}

signed short Magneto::yMagneto() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_Y_H));
	return val;
}

signed short Magneto::zMagneto() const
{
	signed short val = static_cast<signed short>(Private::Wallaby::instance()->readRegister16b(REG_RW_MAG_Z_H));
	return val;
}

Magneto * Magneto::instance()
{
	static Magneto instance;
	return &instance;
}

bool Magneto::calibrate() const
{
	return true; // success
}

Magneto::Magneto()
{

}

Magneto::Magneto(const Magneto & rhs)
{

}

Magneto & Magneto::operator=(const Magneto & rhs)
{
	return *this;
}



#endif /* SRC_MAGNETO_P_CPP_ */

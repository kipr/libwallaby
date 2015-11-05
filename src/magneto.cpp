/*
 * magneto.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/magneto.hpp"
#include "magneto_p.hpp"

signed short Magneto::x()
{
	return Private::Magneto::instance()->xMagneto();
}

signed short Magneto::y()
{
	return Private::Magneto::instance()->yMagneto();
}

signed short Magneto::z()
{
	return Private::Magneto::instance()->zMagneto();
}

bool Magneto::calibrate()
{
	return Private::Magneto::instance()->calibrate();
}

short MagnetoX::value() const
{
	return Private::Magneto::instance()->xMagneto();
}

short MagnetoY::value() const
{
	return Private::Magneto::instance()->yMagneto();
}

short MagnetoZ::value() const
{
	return Private::Magneto::instance()->zMagneto();
}

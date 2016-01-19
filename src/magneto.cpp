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
	return Private::magneto_x(nullptr);
}

signed short Magneto::y()
{
	return Private::magneto_y(nullptr);
}

signed short Magneto::z()
{
	return Private::magneto_z(nullptr);
}

bool Magneto::calibrate()
{
	return Private::magneto_calibrate();
}

short MagnetoX::value() const
{
	return Private::magneto_x(nullptr);
}

short MagnetoY::value() const
{
	return Private::magneto_y(nullptr);
}

short MagnetoZ::value() const
{
	return Private::magneto_z(nullptr);
}

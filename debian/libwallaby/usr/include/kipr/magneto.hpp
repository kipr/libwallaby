/*
 * magneto.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MAGNETO_HPP_
#define INCLUDE_WALLABY_MAGNETO_HPP_

#include "sensor.hpp"

class Magneto
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
};

class MagnetoX : public Sensor<short>
{
public:
	virtual short value() const;
};

class MagnetoY : public Sensor<short>
{
public:
	virtual short value() const;
};

class MagnetoZ : public Sensor<short>
{
public:
	virtual short value() const;
};


#endif /* INCLUDE_WALLABY_MAGNETO_HPP_ */

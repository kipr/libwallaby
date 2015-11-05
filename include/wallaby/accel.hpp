/*
 * accel.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ACCEL_HPP_
#define INCLUDE_WALLABY_ACCEL_HPP_

class Acceleration
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
};

class AccelX
{
public:
	short value() const;
};

class AccelY
{
public:
	short value() const;
};

class AccelZ
{
public:
	short value() const;
};

#endif /* INCLUDE_WALLABY_ACCEL_HPP_ */

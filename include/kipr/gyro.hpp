/*
 * gyro.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GYRO_HPP_
#define INCLUDE_WALLABY_GYRO_HPP_

#include "sensor.hpp"

class Gyro
{
public:
	short gyro_x();
	short gyro_y();
	short gyro_z();
	bool calibrate();
private:
};

class GyroX : public Sensor<short>
{
public:
	virtual short value() const;
};

class GyroY : public Sensor<short>
{
public:
	virtual short value() const;
};

class GyroZ : public Sensor<short>
{
public:
	virtual short value() const;
};


#endif /* INCLUDE_WALLABY_GYRO_HPP_ */

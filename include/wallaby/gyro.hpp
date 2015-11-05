/*
 * gyro.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GYRO_HPP_
#define INCLUDE_WALLABY_GYRO_HPP_


class Gyro
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
};

class GyroX
{
public:
	short value() const;
};

class GyroY
{
public:
	short value() const;
};

class GyroZ
{
public:
	short value() const;
};


#endif /* INCLUDE_WALLABY_GYRO_HPP_ */

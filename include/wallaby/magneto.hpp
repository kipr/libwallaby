/*
 * magneto.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MAGNETO_HPP_
#define INCLUDE_WALLABY_MAGNETO_HPP_


class Magneto
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
};

class MagnetoX
{
public:
	short value() const;
};

class MagnetoY
{
public:
	short value() const;
};

class MagnetoZ
{
public:
	short value() const;
};


#endif /* INCLUDE_WALLABY_MAGNETO_HPP_ */

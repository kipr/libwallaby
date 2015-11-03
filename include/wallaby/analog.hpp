/*
 * analog.hpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ANALOG_HPP_
#define INCLUDE_WALLABY_ANALOG_HPP_

class Analog
{
public:
	Analog(unsigned char port);
	virtual ~Analog();

	virtual unsigned short value() const;

	void setPullup(bool pullup);
	bool pullup() const;

	unsigned char port() const;
private:
	unsigned char m_port;
};

class Analog8 : public Analog
{
public:
	Analog8(unsigned char port);
	virtual ~Analog8();

	unsigned short value() const;
};


#endif /* INCLUDE_WALLABY_ANALOG_HPP_ */

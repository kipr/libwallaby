/*
 * analog.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/analog.hpp"
#include "analog_p.hpp"

Analog::Analog(unsigned char port)
: m_port(port)
{

}

unsigned short Analog::value() const
{
	return Private::Analog::instance()->value(m_port);
}

unsigned char Analog::port() const
{
	return m_port;
}

bool Analog::pullup() const
{
	// Not available on the Wallaby
	return false;
}

Analog8::Analog8(unsigned char port)
: Analog(port)
{

}

unsigned short Analog8::value() const
{
	return Analog::value() >> 4;
}


Analog10::Analog10(unsigned char port)
: Analog(port)
{

}

unsigned short Analog10::value() const
{
	return Analog::value() >> 2;
}

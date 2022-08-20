#include "kipr/analog/analog.hpp"

#include "analog_p.hpp"

using namespace kipr;
using namespace kipr::analog;

Analog::Analog(unsigned char port)
  : m_port(port)
{
}

Analog::~Analog()
{
}

unsigned short Analog::value() const
{
	return analog_value(m_port);
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

void Analog::setPullup(bool pullup)
{

}

Analog8::Analog8(unsigned char port)
: Analog(port)
{

}

unsigned short Analog8::value() const
{
	return Analog::value() >> 4;
}

Analog8::~Analog8()
{

}


Analog10::Analog10(unsigned char port)
: Analog(port)
{

}

Analog10::~Analog10()
{

}

unsigned short Analog10::value() const
{
	return Analog::value() >> 2;
}

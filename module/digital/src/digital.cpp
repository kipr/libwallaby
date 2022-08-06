/*
 * digital.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/digital.hpp"
#include "digital_p.hpp"

Digital::Digital(unsigned char port)
: m_port(port)
{

}

void Digital::setValue(bool value)
{
	Private::set_digital_value(m_port, value);
}

bool Digital::value() const
{
	return Private::digital_value(m_port, nullptr);
}

void Digital::setOutput(bool output)
{
	Private::set_digital_direction(m_port, output);
}

bool Digital::isOutput() const
{
	return Private::digital_output(m_port, nullptr);
}

void Digital::setPullup(bool pullup)
{
	// not available on the Wallaby
}

bool Digital::pullup() const
{
	// not available on the Wallaby
	return false;
}

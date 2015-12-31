/*
 * digital.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/digital.hpp"
#include "battlehill_p.hpp"

Digital::Digital(unsigned char port)
: m_port(port)
{

}

void Digital::setValue(bool value)
{
	Private::BattleHill::instance()->setDigitalValue(m_port, value);
}

bool Digital::value() const
{
	return Private::BattleHill::instance()->getDigitalValue(m_port);
}

void Digital::setOutput(bool output)
{
	Private::BattleHill::instance()->setDigitalOutput(m_port, output);
}

bool Digital::isOutput() const
{
	return Private::BattleHill::instance()->getDigitalOutput(m_port);
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

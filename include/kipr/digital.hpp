/*
 * digital.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_DIGITAL_HPP_
#define INCLUDE_WALLABY_DIGITAL_HPP_

#include "sensor.hpp"

class Digital : public Sensor<bool>
{
public:
	Digital(unsigned char port);

	void setValue(bool value);

	void setOutput(bool output);

	bool isOutput() const;

	void setPullup(bool pullup);

	bool pullup() const;

	virtual bool value() const;

private:
	unsigned char m_port;
};


#endif /* INCLUDE_WALLABY_DIGITAL_HPP_ */

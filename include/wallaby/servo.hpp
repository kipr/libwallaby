/*
 * servo.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_SERVO_HPP_
#define INCLUDE_WALLABY_SERVO_HPP_

class Servo
{
public:
	typedef unsigned int ticks_t;

	Servo(int port);

	void setPosition(ticks_t position);

	ticks_t position() const;

	void disable();
	void enable();
	void setEnabled(bool enabled);
	bool isEnabled() const;
private:
	int m_port;
};

#endif /* INCLUDE_WALLABY_SERVO_HPP_ */

/*
 * servo_p.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_SERVO_P_HPP_
#define SRC_SERVO_P_HPP_


namespace Private
{

class Servo
{
public:
	~Servo();

	void setEnabled(int port, bool enabled);
	bool isEnabled(int port);

	bool setPosition(int port, unsigned short position);
	unsigned short position(int port) const;

	static Servo * instance();

private:
	Servo();
};

}


#endif /* SRC_SERVO_P_HPP_ */

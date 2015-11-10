/*
 * motors_p.hpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_MOTORS_P_HPP_
#define SRC_MOTORS_P_HPP_

#include <stdint.h>

namespace Private
{

class Motor
{
public:
	enum ControlMode
	{
		Inactive = 0,
		Position,
		Speed,
		SpeedPosition
	};

	enum Direction
	{
		PassiveStop = 0,
		Reverse,
		Forward,
		ActiveStop
	};

	~Motor();

	void setPidGains(int port, short p, short i, short d, short pd, short id, short dd);

	void clearBemf(int port);

	void setControlMode(int port, Motor::ControlMode mode);
	Motor::ControlMode controlMode(int port) const;

	bool isPidActive(int port) const;

	void setPidVelocity(int port, int ticks);
	int pidVelocity(int port) const;

	void setPidGoalPos(int port, int pos);
	int pidGoalPos(int port) const;

	//void setControlMode(Motor::ControlMode mode);
	//Motor::ControlMode controlMode() const;

	void pidGains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd) const;

	void setPwm(int port, unsigned short speed); // TODO: note change from char to short
	void setPwmDirection(int port, Motor::Direction dir);

	unsigned short pwm(int port);
	Motor::Direction pwmDirection(int port) const;

	void stop(int port);

	int backEMF(int port); // this is really the integrated back EMF signal

	static Motor * instance();

private:
	Motor();

	int64_t m_cleared[4];

};

}


#endif /* SRC_MOTORS_P_HPP_ */

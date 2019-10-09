/*
 * motors.hpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MOTORS_HPP_
#define INCLUDE_WALLABY_MOTORS_HPP_

class Motor
{
public:
	Motor (int port);

	void clearPositionCounter();

	void moveAtVelocity(short velocity);
	void moveToPosition(short speed, int goalPos);
	void moveRelativePosition(short speed, int deltaPos);

#ifndef SWIG
	void setPidGains(short p, short i, short d, short pd, short id, short dd);
	void pidGains(short & p, short & i, short & d, short & pd, short & id, short & dd);
#endif

	void freeze();

	bool isMotorDone() const;

	void blockMotorDone() const;

	void forward();

	void backward();

	void motor(int percent);

        void baasbennaguui(int percent);

	void motorPower(int percent);

	void off();

	int port() const;

private:
	int m_port;
};


class BackEMF
{
public:
	BackEMF(int port);
	int value() const;
	int port () const;
private:
	int m_port;
};


#endif /* INCLUDE_WALLABY_MOTORS_HPP_ */

/*
 * battlehill_p.hpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Joshua Southerland
 */


#ifndef SRC_BATTLEHILL_P_HPP_
#define SRC_BATTLEHILL_P_HPP_


#include <memory>

#include "daylite/publisher.hpp"
#include "daylite/subscriber.hpp"

namespace Private
{

class BattleHill
{
public:

	enum MotorControlMode
	{
		Inactive = 0,
		Speed,
		Position,
		SpeedPosition
	};

	enum MotorDirection
	{
		PassiveStop = 0,
		Reverse,
		Forward,
		ActiveStop
	};


	static BattleHill * instance();

	virtual ~BattleHill();

	// Ardrone

	// Analog
	unsigned short getAnalogValue(unsigned char port);

	// Battery
	float getBatteryCapacity();
	unsigned short getBatteryRawReading();

	// Buttons - partly from battlehill, partly from harrogate

	// Camera - not from battlehill

	// Create - not from battlehill

	// Digital
	bool getDigitalValue(unsigned int port);
	bool getDigitalOutput(unsigned int port);
	void setDigitalValue(unsigned int port, bool high);
	void setDigitalOutput(unsigned int port, bool output);

	// IMU
	short getAccelX();
	short getAccelY();
	short getAccelZ();
	bool getAccelCalibrated();
	bool calibrateAccel();
	short getGyroX();
	short getGyroY();
	short getGyroZ();
	bool getGyroCalibrated();
	bool calibrateGyro();
	short getMagnetoX();
	short getMagnetoY();
	short getMagnetoZ();
	bool getMagnetoCalibrated();
	bool calibrateMagneto();


	// Motor
	int backEMF(int port);
	void clearBemf(int port);
	void setControlMode(int port, MotorControlMode mode);
	MotorControlMode controlMode(int port);
	bool isPidActive(int port);
	void setPidVelocity(int port, int ticks);
	int pidVelocity(int port);
	void setPidGoalPos(int port, int pos);
	int pidGoalPos(int port);
	void setPidGains(int port, short p, short i, short d, short pd, short id, short dd);
	void pidGains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd);
	void setPwm(int port, unsigned short speed);
	void setPwmDirection(int port, MotorDirection dir);
	unsigned short pwm(int port);
	MotorDirection pwmDirection(int port);
	void stop(int port);


	// Servo
	void enableServo(int port);
	void disableServo(int port);

	void enableServos();
	void disableServos();

	void setServoEnabled(int port, bool enabled);
	bool getServoEnabled(int port);

	unsigned short getServoPosition(int port);
	void setServoPosition(int port, unsigned short position);

private:
	BattleHill();
	bool setup(); // initializes the daylite node
	bool daylite_good_; // is the daylite node alright
	std::shared_ptr<daylite::publisher> set_digital_states_pub_;
};

} /* namespace Private */

#endif /* SRC_BATTLEHILL_P_HPP_ */

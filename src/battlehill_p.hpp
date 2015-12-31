/*
 * battlehill_p.hpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Joshua Southerland
 */


#ifndef SRC_BATTLEHILL_P_HPP_
#define SRC_BATTLEHILL_P_HPP_

namespace Private
{

class BattleHill
{

public:
	static BattleHill * instance();

	virtual ~BattleHill();

	// Ardrone

	// Analog
	unsigned short getAnalogValue(unsigned char port);

	// Battery
	float getBatteryCapacity();
	unsigned short getBatteryRawReading();

	// Camera

	// Create

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

	// Servo



private:
	BattleHill();
	bool setup(); // initializes the daylite node
	bool daylite_good_; // is the daylite node alright
};

} /* namespace Private */

#endif /* SRC_BATTLEHILL_P_HPP_ */

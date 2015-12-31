/*
 * battlehill_p.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Joshua Southerland
 */

#include "battlehill_p.hpp"
#include "robot_p.hpp"

#include <battlecreek/analog_states.hpp>
#include <battlecreek/battery_state.hpp>
#include <battlecreek/digital_states.hpp>
#include <battlecreek/motor_states.hpp>
#include <battlecreek/robot_states.hpp>
#include <battlecreek/servo_states.hpp>


#include <daylite/node.hpp>
#include <daylite/spinner.hpp>

#include <bson.h>
#include <bson_bind/option.hpp>

#include <iostream>
#include <mutex>

using namespace battlecreek;
using namespace daylite;
using namespace std;

namespace
{
	std::mutex battlehill_mutex;

	template<typename T>
	inline bson_bind::option<T> safe_unbind(const bson_t *raw_msg)
	{
		using namespace bson_bind;
		T ret;
		try
		{
			ret = T::unbind(raw_msg);
		}
		catch(const invalid_argument &e)
		{
			cerr << e.what() << endl;
			return none<T>();
		}

		return some(ret);
	}

	// TODO: move to namespace / class
	void robot_states_cb(const bson_t *raw_msg, void *)
	{
		const auto msg_option = safe_unbind<robot_states>(raw_msg);
		if(msg_option.none()) return;

		auto msg = msg_option.unwrap();

		Private::Robot::instance()->setRobotStates(msg);
	}

	void exhaust_spinner()
	{
		// TODO: spin_once as long as we have messages
		spinner::spin_once();
	}
}

namespace Private
{

unsigned short BattleHill::getAnalogValue(unsigned char port)
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().analog_states.value[port];
}

float BattleHill::getBatteryCapacity()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().battery_state.capacity;
}

unsigned short BattleHill::getBatteryRawReading()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().battery_state.raw_adc;
}

bool getDigitalValue(unsigned int port)
{
	exhaust_spinner();
	// TODO: bounds checking
	return Private::Robot::instance()->getRobotStates().digital_states.value[port];
}

bool getDigitalOutput(unsigned int port)
{
	exhaust_spinner();
	// TODO: bounds checking
	return Private::Robot::instance()->getRobotStates().digital_states.output[port];
}

void setDigitalValue(unsigned int port, bool high)
{
	// FIXME: implement
}

void setDigitalOutput(unsigned int port, bool output)
{
	// FIXME: implement
}

short BattleHill::getAccelX()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.accel_state.x;
}

short BattleHill::getAccelY()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.accel_state.y;
}

short BattleHill::getAccelZ()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.accel_state.z;
}

bool BattleHill::getAccelCalibrated()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.accel_state.calibrated;
}

bool BattleHill::calibrateAccel()
{
	// TODO: we don't support this yet
	std::cout << "Accelerometer calibration not yet supported" << std::endl;
	return true;
}

short BattleHill::getGyroX()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.gyro_state.x;
}

short BattleHill::getGyroY()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.gyro_state.y;
}

short BattleHill::getGyroZ()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.gyro_state.z;
}

bool BattleHill::getGyroCalibrated()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.gyro_state.calibrated;
}

bool BattleHill::calibrateGyro()
{
	// TODO: we don't support this yet
	std::cout << "Gyrometer calibration not yet supported" << std::endl;
	return true;
}

short BattleHill::getMagnetoX()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.magneto_state.x;
}

short BattleHill::getMagnetoY()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.magneto_state.y;
}

short BattleHill::getMagnetoZ()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.magneto_state.z;
}

bool BattleHill::getMagnetoCalibrated()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().imu_state.calibrated;
}

bool BattleHill::calibrateMagneto()
{
	// TODO: we don't support this yet
	std::cout << "Magnetometer calibration not yet supported" << std::endl;
	return true;
}

bool BattleHill::setup()
{
	std::lock_guard<std::mutex> lock(battlehill_mutex);

	// TODO: mutex lock this for  thread safety
	static auto n = node::create_node("libwallaby");

	if (!n->start("127.0.0.1", 8374))
	{
		std::cerr << "Failed to contact daylite master" << std::endl;
		return false;
	}

	static auto robot_states_sub = n->subscribe("robot/robot_states", &robot_states_cb);

	return true;
}

BattleHill::BattleHill()
: daylite_good_(false)
{

}

BattleHill::~BattleHill()
{

}

BattleHill * BattleHill::instance()
{
	static BattleHill instance;

	if (!(instance.daylite_good_)) instance.daylite_good_ = instance.setup();
	return &instance;
}

} /* namespace Private */

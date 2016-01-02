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
#include <battlecreek/set_digital_state.hpp>
#include <battlecreek/set_servo_state.hpp>



#include <daylite/node.hpp>
#include <daylite/spinner.hpp>

#include <bson.h>
#include <bson_bind/option.hpp>

#include <iostream>
#include <mutex>

using namespace battlecreek;
using namespace daylite;
using namespace std;

// TODO: move these and share
static const unsigned int NUM_SERVOS = 4;
static const unsigned int NUM_ADC = 6;
static const unsigned int NUM_DIG = 16;

namespace
{
std::mutex battlehill_mutex;

template<typename T>
inline bson_bind::option<T> safe_unbind(const bson & raw_msg)
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
void robot_states_cb(const bson & raw_msg, void *)
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

unsigned long int BattleHill::getRobotStatesSequenceNumber()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().seq;
}

unsigned long int BattleHill::getRobotUpdateCount()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().update_count;
}

unsigned short BattleHill::getAnalogValue(unsigned char port)
{
	exhaust_spinner();
	if (port >= Private::Robot::instance()->getRobotStates().analog_states.value.size()) return 0; // TODO: feedback
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

bool BattleHill::getDigitalValue(unsigned int port)
{
	exhaust_spinner();
	if (port >= Private::Robot::instance()->getRobotStates().digital_states.value.size()) return false;// TODO: feedback
	return Private::Robot::instance()->getRobotStates().digital_states.value[port];
}

bool BattleHill::getDigitalOutput(unsigned int port)
{
	exhaust_spinner();
	if (port >= Private::Robot::instance()->getRobotStates().digital_states.output.size()) return false;// TODO: feedback
	return Private::Robot::instance()->getRobotStates().digital_states.output[port];
}

void BattleHill::setDigitalValue(unsigned int port, bool high)
{
	battlecreek::set_digital_state msg;
	msg.port = port;
	msg.value = high;

	set_digital_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::setDigitalOutput(unsigned int port, bool output)
{
	battlecreek::set_digital_state msg;
	msg.port = port;
	msg.output = output;

	set_digital_state_pub_->publish(msg.bind());
	spinner::spin_once();
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


int BattleHill::backEMF(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3) return 0;
	int val = 0; // TODO: int val = Private::Wallaby::instance()->readRegister32b(REG_RW_MOT_0_B3 + 4 * port);
	return Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].position;
}

void BattleHill::clearBemf(int port)
{
	if (port < 0 || port > 3) return;
	//FIXME: implement: Private::Wallaby::instance()->writeRegister32b(REG_RW_MOT_0_B3 + 4 * port, 0);
}

void BattleHill::setControlMode(int port, BattleHill::MotorControlMode mode)
{
	/** FIXME
	if (port < 0 || port > 3) return;
	unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES);

	const unsigned short offset = 2*port;

	// clear old drive mode
	modes &= ~(0x3 << offset);

	// set new drive mode
	modes |=  (((int)mode) << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_MODES, modes);
	 */
}

BattleHill::MotorControlMode BattleHill::controlMode(int port)
{
	/** FIXME
	if (port < 0 || port > 3) return Motor::ControlMode::Inactive; // TODO: better fail code
	unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES);

	const unsigned short offset = 2*port;

	//
	unsigned char mode = (modes & (0x3 << offset)) >> offset;

	return (Private::Motor::ControlMode)mode;
	 */
	return BattleHill::MotorControlMode::Inactive; // FIXME
}

bool BattleHill::isPidActive(int port)
{
	//unsigned char motor_done = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DONE);
	//return !(motor_done & (1 < port));
	return false; // FIXME
}

void BattleHill::setPidVelocity(int port, int ticks)
{
	/* FIXME
	// TODO:check that byte order doesn't get messed up
	// TODO: may need to put some logic in for not writing goals if they equal the current goal
	//  ... maybe add on the co-proc?
	unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * port; // TODO: 32 bit?
	Private::Wallaby::instance()->writeRegister16b(goal_addy, static_cast<signed short>(ticks));
	 */
}

int BattleHill::pidVelocity(int port)
{
	/* FIXME
	// TODO: 32 bit?
	unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * port;
	int val = Private::Wallaby::instance()->readRegister16b(goal_addy);
	return val;
	 */
	return 0;//FIXME
}

void BattleHill::setPidGoalPos(int port, int pos)
{
	/* FIXME
	// TODO: do we really need 64 bit positions?
	// TODO: logic to not set goals if they match the current value (in co-proc firmware maybe?)
	unsigned int goal_addy = REG_W_MOT_0_GOAL_B3 + 4 * port;
	Private::Wallaby::instance()->writeRegister32b(goal_addy, pos);
	 */
}


int BattleHill::pidGoalPos(int port)
{
	// FIXME: last commanded goal pos since we can't read it
	return 0;
}

void BattleHill::setPidGains(int port, short p, short i, short d, short pd, short id, short dd)
{
	if (port < 0 || port > 3) return;
	/* FIXME
	unsigned int addy_offset = 12 * port;  // 6 registers at 2 bytes each... per port
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_P_H + addy_offset, p);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_I_H + addy_offset, i);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_D_H + addy_offset, d);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_PD_H + addy_offset, pd);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_ID_H + addy_offset, id);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_DD_H + addy_offset, dd);
	 */
}

void BattleHill::pidGains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	if (port < 0 || port > 3) return;
	//unsigned short offset = port * 2 * 3;
	//TODO we can't read them from co-proc
}

void BattleHill::setPwm(int port, unsigned short speed)
{
	/* FIXME
	if (port < 0 || port > 3) return;
	// TODO: error signal outside of range
	setControlMode(port, Private::Motor::Inactive);
	const unsigned short speedMax = 400;
	unsigned short adjustedSpeed = speed * 4;
	if (adjustedSpeed > speedMax) adjustedSpeed = speedMax; // TODO: check scaling (1/4 percent increments)
	Private::Wallaby::instance()->writeRegister16b(REG_RW_MOT_0_PWM_H + 2 * port, adjustedSpeed);
	 */
}

void BattleHill::setPwmDirection(int port, MotorDirection dir)
{
	/* FIXME
	if (port < 0 || port > 3) return;

	setControlMode(port, Private::Motor::Inactive);

	unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS);

	unsigned short offset = 2 * port;

	dirs &= ~(0x3 << offset);

	dirs |= (dir << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_DIRS, dirs);
	 */
}

unsigned short BattleHill::pwm(int port)
{
	/* FIXME
	if (port < 0 || port > 3) return 0;
	// TODO: error signal outside of range
	return Private::Wallaby::instance()->readRegister16b(REG_RW_MOT_0_PWM_H + 2 * port);
	 */
	return 0; // TODO
}

BattleHill::MotorDirection BattleHill::pwmDirection(int port)
{
	/* FIXME
	if (port < 0 || port > 3) return Private::Motor::Direction::PassiveStop;
	// TODO: error signal outside of range

	unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS);

	unsigned short offset = 2 * port;

	return (MotorDirection)((dirs & (0x3 << offset)) >> offset);
	 */
	return MotorDirection::PassiveStop; // FIXME
}

void BattleHill::stop(int port)
{
	/* FIXME
	if (port < 0 || port > 3) return;
	setControlMode(port, Private::Motor::Inactive);
	setPwmDirection(port, PassiveStop);
	 */
}


void BattleHill::enableServo(int port)
{
	battlecreek::set_servo_state msg;
	msg.port = port;
	msg.enabled = true;

	set_servo_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::disableServo(int port)
{
	battlecreek::set_servo_state msg;
	msg.port = port;
	msg.enabled = true;

	set_servo_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::enableServos()
{
	// TODO: there is actually only one enable/disable setting for all servos
	// ... here I have decided to re-use enable_servo in case that changes
	for (unsigned int i = 0; i < NUM_SERVOS; ++i)
	{
		enableServo(i);
	}

}
void BattleHill::disableServos()
{
	// TODO: there is actually only one enable/disable setting for all servos
	// ... here I have decided to re-use enable_servo in case that changes
	for (unsigned int i = 0; i < NUM_SERVOS; ++i)
	{
		disableServo(i);
	}
}

void BattleHill::setServoEnabled(int port, bool enabled)
{
	if (enabled)
	{
		enableServo(port);
	}
	else
	{
		disableServo(port);
	}
}

bool BattleHill::getServoEnabled(int port)
{
	exhaust_spinner();
	if (port >= NUM_SERVOS) return false; // TODO
	return Private::Robot::instance()->getRobotStates().servo_states.enabled[port];
}

unsigned short BattleHill::getServoPosition(int port)
{
	exhaust_spinner();
	if (port >= NUM_SERVOS) return 0xFFFF;  // TODO
	return Private::Robot::instance()->getRobotStates().servo_states.position[port];
}

void BattleHill::setServoPosition(int port, unsigned short position)
{
	battlecreek::set_servo_state msg;
	msg.port = port;
	msg.position = position;

	set_servo_state_pub_->publish(msg.bind());
	spinner::spin_once();
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

	set_digital_state_pub_ = n->advertise("robot/set_digital_state");
	set_servo_state_pub_ = n->advertise("robot/set_servo_state");

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

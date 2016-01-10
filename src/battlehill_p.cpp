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
#include <battlecreek/set_battlehill_state.hpp>
#include <battlecreek/set_digital_state.hpp>
#include <battlecreek/set_motor_state.hpp>
#include <battlecreek/set_pid_state.hpp>
#include <battlecreek/set_servo_state.hpp>



#include <daylite/node.hpp>
#include <daylite/spinner.hpp>

#include <bson.h>
#include <bson_bind/option.hpp>

#include <iostream>
#include <mutex>
#include <unistd.h>

using namespace battlecreek;
using namespace daylite;
using namespace std;

// TODO: move these and share
static const unsigned int NUM_ADC = 6;
static const unsigned int NUM_DIG = 16;
static const unsigned int NUM_MOTORS = 4;
static const unsigned int NUM_SERVOS = 4;


static bool have_robot_data = false;  // TODO: move to class and bind callback

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
	have_robot_data = true; // TODO: make this a class var
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

unsigned short BattleHill::getRobotFirmwareVersion()
{
	exhaust_spinner();
	return Private::Robot::instance()->getRobotStates().firmware_version;
}

bool BattleHill::setRobotUpdateDelay(unsigned int us_delay)
{
	battlecreek::set_battlehill_state msg;
	msg.update_delay = us_delay;

	set_battlehill_state_pub_->publish(msg.bind());
	spinner::spin_once();

	return true; // TODO: should we limit the range?  Battlehill can handle delays of 0us
}

unsigned int BattleHill::getRobotUpdateDelay()
{
	exhaust_spinner();
	return Private::Robot::instance()->getBattlehillState().update_delay;
}

bool BattleHill::setLowVoltThreshold(float volts)
{
	battlecreek::set_battlehill_state msg;
	msg.low_voltage_threshold = volts;

	set_battlehill_state_pub_->publish(msg.bind());
	spinner::spin_once();

	return true; // TODO: should we limit the range?  -1.0V might be a useful setting
}

float BattleHill::getLowVoltThreshold()
{
	exhaust_spinner();
	return Private::Robot::instance()->getBattlehillState().low_voltage_threshold;
}


unsigned short BattleHill::getAnalogValue(unsigned char port)
{
	exhaust_spinner();
	if (port >= Private::Robot::instance()->getRobotStates().analog_states.value.size()) return 0; // TODO: feedback
	return Private::Robot::instance()->getRobotStates().analog_states.value[port] * 4; // FIXME: should not be scaling here and in battlehill, only in the web browser
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
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size()) return 0;
	return Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].position;
}

void BattleHill::clearBemf(int port)
{
	if (port < 0 || port > 3) return;

	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.reset_position = true;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::setControlMode(int port, BattleHill::MotorControlMode mode)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.mode  = (uint8_t)mode;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

BattleHill::MotorControlMode BattleHill::controlMode(int port)
{
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size() ) return BattleHill::MotorControlMode::Inactive; // TODO

	return (BattleHill::MotorControlMode)(Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].mode);
}

bool BattleHill::isPidActive(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size()) return false; // TODO
	return Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].pid_active;
}

void BattleHill::setPidVelocity(int port, int ticks)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.velocity_goal = ticks;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

int BattleHill::pidVelocity(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size()) return 0;
	return Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].goal_velocity;
}

void BattleHill::setPidGoalPos(int port, int pos)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.position_goal = pos;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}


int BattleHill::pidGoalPos(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3) return 0; // TODO
	return Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].goal_position;
}

void BattleHill::setPidGains(int port, short p, short i, short d, short pd, short id, short dd)
{
	if (port < 0 || port > 3) return;

	battlecreek::set_pid_state msg;
	msg.port = port;
	msg.p = p;
	msg.i = i;
	msg.d = d;
	msg.pd = pd;
	msg.id = id;
	msg.dd = dd;

	set_pid_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::pidGains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd)
{
	exhaust_spinner();
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().pid_states.pid_state.size()) return;
	//unsigned short offset = port * 2 * 3;
	//TODO we can't read them from co-proc
	p = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].p;
	i = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].i;
	d = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].d;
	pd = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].pd;
	id = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].id;
	dd = Private::Robot::instance()->getRobotStates().pid_states.pid_state[port].dd;
}

void BattleHill::setPwm(int port, unsigned short speed)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.power = speed;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

void BattleHill::setPwmDirection(int port, MotorDirection dir)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.direction = (uint8_t)dir;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
}

unsigned short BattleHill::pwm(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size()) return 0;
	return (MotorDirection)(Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].power);
}

BattleHill::MotorDirection BattleHill::pwmDirection(int port)
{
	exhaust_spinner();
	if (port < 0 || port > 3 || port >= Private::Robot::instance()->getRobotStates().motor_states.motor_state.size()) return MotorDirection::PassiveStop;
	return (MotorDirection)(Private::Robot::instance()->getRobotStates().motor_states.motor_state[port].direction);
}

void BattleHill::stop(int port)
{
	battlecreek::set_motor_state msg;
	msg.port = port;
	msg.stop = true;

	set_motor_state_pub_->publish(msg.bind());
	spinner::spin_once();
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
	msg.enabled = false;

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
	if (port >= NUM_SERVOS || port >= Private::Robot::instance()->getRobotStates().servo_states.enabled.size()) return false; // TODO
	return Private::Robot::instance()->getRobotStates().servo_states.enabled[port];
}

unsigned short BattleHill::getServoPosition(int port)
{
	exhaust_spinner();
	if (port >= NUM_SERVOS || port >= Private::Robot::instance()->getRobotStates().servo_states.position.size()) return 0xFFFF;  // TODO
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
	node_ = node::create_node("libwallaby");

	// We have to set auto exit to true to give daylite permission to close the program
	// if Ctrl-C is pressed
	node_->set_auto_exit(true);

	if (!node_->start("127.0.0.1", 8374))
	{
		std::cerr << "Failed to contact daylite master" << std::endl;
		return false;
	}


	set_battlehill_state_pub_ = node_->advertise("battlehill/set_battlehill_state");
	set_digital_state_pub_ = node_->advertise("robot/set_digital_state");
	set_motor_state_pub_ = node_->advertise("robot/set_motor_state");
	set_pid_state_pub_ = node_->advertise("robot/set_pid_state");
	set_servo_state_pub_ = node_->advertise("robot/set_servo_state");


	static auto robot_states_sub = node_->subscribe("robot/robot_states", &robot_states_cb);

	// wait for the subscriber to get a packet so we have real data to back library calls
	while(!have_robot_data) usleep(1000);

	return true;
}

BattleHill::BattleHill()
: daylite_good_(false)
{

}

BattleHill::~BattleHill()
{
	if (daylite_good_)
	{
		// daylite was set up. we need to make sure any messages waiting to be published are published
		while(node_->out_queue_count() > 0) usleep(100000);
	}
}

BattleHill * BattleHill::instance()
{
	static BattleHill instance;

	if (!(instance.daylite_good_)) instance.daylite_good_ = instance.setup();
	return &instance;
}

} /* namespace Private */

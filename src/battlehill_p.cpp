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

// TODO: relocate?
static const unsigned int NUM_ADC = 6;

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
}

namespace Private
{

float BattleHill::getBatteryCapacity()
{
	spinner::spin_once();
	return Private::Robot::instance()->getRobotStates().battery_state.capacity;
}

unsigned short BattleHill::getBatteryRawReading()
{
	spinner::spin_once();
	return Private::Robot::instance()->getRobotStates().battery_state.raw_adc;
}

unsigned short BattleHill::getAnalogValue(unsigned char port)
{
	spinner::spin_once();
	// TODO: bounds checking
	if (port >= NUM_ADC) return 0; // TODO: error feedback
	return Private::Robot::instance()->getRobotStates().analog_states.value[port];
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

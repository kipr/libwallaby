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


// TODO: move to class
static float capacity = 0.f;
static const unsigned int NUM_ADC = 6;
static unsigned short adc_vals[NUM_ADC] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

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
		capacity = msg.battery_state.capacity;

		for (unsigned int i = 0; i < NUM_ADC; ++i)
		{
			// FIXME: handle missing adc from message
			adc_vals[i] = msg.analog_states.value[i];
		}
	}
}

namespace Private
{

float BattleHill::getBatteryCapacity()
{
	spinner::spin_once();
	return capacity;
}

unsigned short BattleHill::getAnalogValue(unsigned char port)
{
	spinner::spin_once();
	// TODO: bounds checking
	if (port >= NUM_ADC) return 0; // TODO: error feedback
	return adc_vals[port];
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

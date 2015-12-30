/*
 * battlehill_p.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Joshua Southerland
 */

#include "battlehill_p.hpp"


#include <battlecreek/analog_states.hpp>
#include <battlecreek/battery_state.hpp>
#include <battlecreek/digital_states.hpp>
#include <battlecreek/motor_states.hpp>
#include <battlecreek/servo_states.hpp>

#include <daylite/node.hpp>
#include <daylite/spinner.hpp>

#include <bson.h>
#include <bson_bind/option.hpp>

#include <iostream>

using namespace battlecreek;
using namespace daylite;
using namespace std;


// TODO: move to class
static float capacity = 0.f;

namespace
{
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
void battery_state_cb(const bson_t *raw_msg, void *)
{
	const auto msg_option = safe_unbind<battery_state>(raw_msg);
	if(msg_option.none()) return;

	auto msg = msg_option.unwrap();
	capacity = msg.capacity;
}
}

namespace Private
{

float BattleHill::getBatteryCapacity()
{
	spinner::spin_once();
	return capacity;
}

bool BattleHill::setup()
{
	static auto n = node::create_node("libwallaby");

	if (!n->start("127.0.0.1", 8374))
	{
		std::cerr << "Failed to contact daylite master" << std::endl;
		return false;
	}

	static auto battery_state_sub = n->subscribe("robot/battery_state", &battery_state_cb);

	return true;
}

BattleHill::BattleHill()
: good(false)
{

}

BattleHill::~BattleHill()
{

}

BattleHill * BattleHill::instance()
{
	static BattleHill instance;

	if (!(instance.good)) instance.good = instance.setup();
	return &instance;
}

} /* namespace Private */

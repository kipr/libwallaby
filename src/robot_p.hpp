/*
 * robot_p.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_ROBOT_P_HPP_
#define SRC_ROBOT_P_HPP_

#include "battlecreek/robot_states.hpp"
#include "battlecreek/battlehill_state.hpp"

namespace Private
{

class Robot
{
public:
	virtual ~Robot();
	static Robot * instance();

	const battlecreek::robot_states & getRobotStates() const;
	void setRobotStates(const battlecreek::robot_states & robot_states);

	const battlecreek::battlehill_state & getBattlehillState() const;
	void setBattlehillState(const battlecreek::battlehill_state & battlehill_state);

private:
	Robot();
	battlecreek::robot_states robot_states_;
	battlecreek::battlehill_state battlehill_state_;
};

} /* namespace Private */

#endif /* SRC_ROBOT_P_HPP_ */

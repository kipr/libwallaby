/*
 * robot_p.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#include "robot_p.h"

namespace Private
{

Robot::Robot()
{
	// TODO Auto-generated constructor stub

}

Robot::~Robot()
{
	// TODO Auto-generated destructor stub
}

Robot * Robot::instance()
{
	static Robot instance;

	return &instance;
}

} /* namespace Private */

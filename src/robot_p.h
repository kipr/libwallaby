/*
 * robot_p.h
 *
 *  Created on: Dec 31, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_ROBOT_P_H_
#define SRC_ROBOT_P_H_

namespace Private
{

class Robot
{
public:
	virtual ~Robot();
	static Robot * instance();

private:
	Robot();
};

} /* namespace Private */

#endif /* SRC_ROBOT_P_H_ */

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

	float getBatteryCapacity();

private:
	BattleHill();

};

} /* namespace Private */

#endif /* SRC_BATTLEHILL_P_HPP_ */

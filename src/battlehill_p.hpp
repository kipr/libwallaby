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

	unsigned short BattleHill::getAnalogValue(unsigned char port);

private:
	BattleHill();
	bool setup(); // initializes the daylite node
	bool daylite_good_; // is the daylite node alright
};

} /* namespace Private */

#endif /* SRC_BATTLEHILL_P_HPP_ */

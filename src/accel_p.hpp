/*
 * accel_p.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_ACCEL_P_HPP_
#define SRC_ACCEL_P_HPP_



namespace Private
{

class Accel
{
public:
	~Accel();

	signed short xAccel() const;
	signed short yAccel() const;
	signed short zAccel() const;

	static Accel * instance();

	bool calibrate() const;

private:
	Accel();
	Accel(const Accel & rhs);
	Accel & operator=(const Accel & rhs);
};

}



#endif /* SRC_ACCEL_P_HPP_ */

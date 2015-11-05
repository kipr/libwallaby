/*
 * gyro_p.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_GYRO_P_HPP_
#define SRC_GYRO_P_HPP_


namespace Private
{

class Gyro
{
public:
	~Gyro();

	signed short xGyro() const;
	signed short yGyro() const;
	signed short zGyro() const;

	static Gyro * instance();

	bool calibrate() const;

private:
	Gyro();
	Gyro(const Gyro & rhs);
	Gyro & operator=(const Gyro & rhs);
};

}


#endif /* SRC_GYRO_P_HPP_ */

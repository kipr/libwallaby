/*
 * digital_p.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_DIGITAL_P_HPP_
#define SRC_DIGITAL_P_HPP_

namespace Private
{

class Digital
{
public:
	enum Direction {
		In,
		Out,
		Unknown
	};

	~Digital();

	bool value(unsigned char port) const;
	bool setValue(unsigned char port, bool value);

	const Digital::Direction direction(unsigned char port) const;
	bool setDirection(unsigned char port, const Digital::Direction & direction);

	static Digital * instance();

private:
	void wiggle() const;

	Digital();

	Digital(const Digital & rhs);
	Digital & operator=(const Digital & rhs);
};

}



#endif /* SRC_DIGITAL_P_HPP_ */

/*
 * magneto_p.hpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_MAGNETO_P_HPP_
#define SRC_MAGNETO_P_HPP_


namespace Private
{

class Magneto
{
public:
	~Magneto();

	signed short xMagneto() const;
	signed short yMagneto() const;
	signed short zMagneto() const;

	static Magneto * instance();

	bool calibrate() const;

private:
	Magneto();
	Magneto(const Magneto & rhs);
	Magneto & operator=(const Magneto & rhs);
};

}


#endif /* SRC_MAGNETO_P_HPP_ */

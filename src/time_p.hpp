/*
 * time_p.hpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_TIME_P_HPP_
#define SRC_TIME_P_HPP_


namespace Private
{
	namespace Time
	{
		void microsleep(unsigned long microseconds);
		unsigned long systime();
	}
}


#endif /* SRC_TIME_P_HPP_ */

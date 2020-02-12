/*
 * sensor.hpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_SENSOR_HPP_
#define INCLUDE_WALLABY_SENSOR_HPP_

#include "export.h"

template<typename T>
#ifdef SWIG
class Sensor
#else
class EXPORT_SYM Sensor
#endif
{
public:
	virtual ~Sensor() {}

	virtual T value() const = 0;
};


#endif /* INCLUDE_WALLABY_SENSOR_HPP_ */

/*
 * sensor.hpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef _KIPR_SENSOR_SENSOR_HPP_
#define _KIPR_SENSOR_SENSOR_HPP_

#include "kipr/export/export.h"

namespace kipr
{
  namespace sensor
  {
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
  }
}

#endif
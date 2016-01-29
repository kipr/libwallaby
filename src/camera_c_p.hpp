/*
 * camera_c_p.hpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#ifndef _CAMERA_C_P_HPP_
#define _CAMERA_C_P_HPP_

#include "wallaby/camera.hpp"

namespace Private
{
  class DeviceSingleton
  {
  public:    
    static ::Camera::Device *instance();
    
  private:
    static ::Camera::Device *s_device;
  };
}

#endif
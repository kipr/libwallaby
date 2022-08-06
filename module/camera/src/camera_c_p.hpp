#ifndef _KIPR_CAMERA_CAMERA_C_P_HPP_
#define _KIPR_CAMERA_CAMERA_C_P_HPP_

#include "kipr/camera/camera.hpp"

namespace kipr
{
  namespace camera
  {
    class DeviceSingleton
    {
    public:    
      static Device *instance();
      
    private:
      static Device *s_device;
    };
  }
}

#endif
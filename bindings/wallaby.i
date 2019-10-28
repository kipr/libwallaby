%module wallaby
%{
  //#include "kovan.hpp"
  //#include "kovan.h"
  #include "kipr/wallaby.hpp"
  #include "kipr/wallaby.h"
  //#include "segment.hpp"
  //#include "colinear_segmenter.hpp"
  //#include "xtion_depth_driver.hpp"
  
  #include "kipr/motors.hpp"
  
  #ifdef SWIGPYTHON
  #define _open open
  #endif
  
  //#define CameraDevice Device
  
  //using namespace depth;
%}

#define EXPORT_SYM

#ifdef SWIGPYTHON
#define open _open
#endif

%include "vtable.h"

%include "button_prelude.i"
//%include "camera_prelude.i"

%include "sensor.hpp"
%template(IntSensor) Sensor<int>;
%template(ShortSensor) Sensor<short>;
%template(UnsignedShortSensor) Sensor<unsigned short>;
%template(BoolSensor) Sensor<bool>;
%include "motors.hpp"
%include "geom.hpp"
//%include "port.hpp"
//%include "segment.hpp"
//%template(IntSegmenter) Segmenter<int>;
%include "servo.hpp"
%include "util.hpp"
//%include "create.hpp"
%include "color.hpp"
%include "battery.hpp"
%include "analog.hpp"
%include "accel.hpp"
%include "compass.hpp"
%include "gyro.hpp"
%include "magneto.hpp"
//%include "camera.hpp"
%include "button.hpp"
//%include "ardrone.hpp"
%include "config.hpp"
//%include "datalog.hpp"
//%include "colinear_segmenter.hpp"
%include "digital.hpp"
%include "graphics.h"

#ifndef SWIGJAVA
%include "thread.hpp"
#endif

%include "motors.h"
%include "geom.h"
%include "graphics.h"
%include "servo.h"
%include "util.h"
%include "create.h"
//%include "depth.h"
%include "battery.h"
%include "analog.h"
%include "general.h"
%include "accel.h"
%include "compass.h"
%include "gyro.h"
%include "magneto.h"
%include "camera.h"
%include "button.h"
%include "botball.h"
//%include "ardrone.h"
//%include "datalog.h"
%include "console.h"
%include "audio.h"
%include "thread.h"
%include "digital.h"

//%include "camera.i"
//%include "create.i"
//%include "motor.i"


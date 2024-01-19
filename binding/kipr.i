%module kipr

%include "kipr/config.h"

#define EXPORT_SYM

#ifdef KIPR_MODULE_ACCEL
%include "module/accel/binding.i"
#endif

#ifdef KIPR_MODULE_ANALOG
%include "module/analog/binding.i"
#endif

#ifdef KIPR_MODULE_AUDIO
%include "module/audio/binding.i"
#endif

#ifdef KIPR_MODULE_BATTERY
%include "module/battery/binding.i"
#endif

#ifdef KIPR_MODULE_BOTBALL
%include "module/botball/binding.i"
#endif

#ifdef KIPR_MODULE_BUTTON
%include "module/button/binding.i"
#endif

#ifdef KIPR_MODULE_CAMERA
%include "module/camera/binding.i"
#endif

#ifdef KIPR_MODULE_COMPASS
%include "module/compass/binding.i"
#endif

#ifdef KIPR_MODULE_CONSOLE
%include "module/console/binding.i"
#endif

#ifdef KIPR_MODULE_CORE
%include "module/core/binding.i"
#endif

#ifdef KIPR_MODULE_CREATE
%include "module/create/binding.i"
#endif

#ifdef KIPR_MODULE_DIGITAL
%include "module/digital/binding.i"
#endif

#ifdef KIPR_MODULE_GEOMETRY
%include "module/geometry/binding.i"
#endif

#ifdef KIPR_MODULE_GRAPHICS
%include "module/graphics/binding.i"
#endif

#ifdef KIPR_MODULE_GYRO
%include "module/gyro/binding.i"
#endif

#ifdef KIPR_MODULE_MAGNETO
%include "module/magneto/binding.i"
#endif

#ifdef KIPR_MODULE_MOTOR
%include "module/motor/binding.i"
#endif

#ifdef KIPR_MODULE_SERVO
%include "module/servo/binding.i"
#endif

#ifdef KIPR_MODULE_TELLO
%include "module/tello/binding.i"
#endif

#ifdef KIPR_MODULE_TIME
%include "module/time/binding.i"
#endif

#ifdef KIPR_MODULE_WAIT_FOR
%include "module/wait_for/binding.i"
#endif

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



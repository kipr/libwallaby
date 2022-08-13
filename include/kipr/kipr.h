#ifndef _KIPR_KIPR_H_
#define _KIPR_KIPR_H_

#include "kipr/config.h"

#ifdef KIPR_MODULE_ACCEL
#include "kipr/accel/accel.h"
#endif

#ifdef KIPR_MODULE_ANALOG
#include "kipr/analog/analog.h"
#endif

#ifdef KIPR_MODULE_AUDIO
#include "kipr/audio/audio.h"
#endif

#ifdef KIPR_MODULE_BATTERY
#include "kipr/battery/battery.h"
#endif

#ifdef KIPR_MODULE_BOTBALL
#include "kipr/botball/botball.h"
#endif

#ifdef KIPR_MODULE_BUTTON
#include "kipr/button/button.h"
#endif

#ifdef KIPR_MODULE_CAMERA
#include "kipr/camera/camera.h"
#endif

#ifdef KIPR_MODULE_COMPASS
#include "kipr/compass/compass.h"
#endif

#ifdef KIPR_MODULE_CONSOLE
#include "kipr/console/console.h"
#include "kipr/console/display.h"
#endif

#ifdef KIPR_MODULE_CREATE
#include "kipr/create/create.h"
#endif

#ifdef KIPR_MODULE_DIGITAL
#include "kipr/digital/digital.h"
#endif

#ifdef KIPR_MODULE_GRAPHICS
#include "kipr/graphics/graphics.h"
#include "kipr/graphics/graphics_characters.h"
#include "kipr/graphics/graphics_key_code.h"
#endif

#ifdef KIPR_MODULE_GYRO
#include "kipr/gyro/gyro.h"
#endif

#ifdef KIPR_MODULE_MAGNETO
#include "kipr/magneto/magneto.h"
#endif

#ifdef KIPR_MODULE_MOTOR
#include "kipr/motor/motor.h"
#endif

#ifdef KIPR_MODULE_SERVO
#include "kipr/servo/servo.h"
#endif

#ifdef KIPR_MODULE_THREAD
#include "kipr/thread/thread.h"
#endif

#ifdef KIPR_MODULE_TIME
#include "kipr/time/time.h"
#endif

#include <stdio.h>
#include <math.h>

#endif


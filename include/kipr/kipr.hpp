#ifndef _KIPR_KIPR_HPP_
#define _KIPR_KIPR_HPP_

#include "kipr/config.h"

#ifdef KIPR_MODULE_ACCEL
#include "kipr/accel/accel.hpp"
#endif

#ifdef KIPR_MODULE_ANALOG
#include "kipr/analog/analog.hpp"
#endif

#ifdef KIPR_MODULE_AUDIO
#include "kipr/audio/audio.h"
#endif

#ifdef KIPR_MODULE_BATTERY
#include "kipr/battery/battery.hpp"
#endif

#ifdef KIPR_MODULE_BOTBALL
#include "kipr/botball/botball.h"
#endif

#ifdef KIPR_MODULE_BUTTON
#include "kipr/button/button.hpp"
#endif

#ifdef KIPR_MODULE_CAMERA
#include "kipr/camera/camera.hpp"
#endif

#ifdef KIPR_MODULE_COMPASS
#include "kipr/compass/compass.hpp"
#endif

#ifdef KIPR_MODULE_CONSOLE
#include "kipr/console/console.hpp"
#include "kipr/console/display.h"
#endif

#ifdef KIPR_MODULE_CREATE
#include "kipr/create/create.hpp"
#endif


#ifdef KIPR_MODULE_DIGITAL
#include "kipr/digital/digital.hpp"
#endif

#ifdef KIPR_MODULE_GRAPHICS
#include "kipr/graphics/graphics.h"
#include "kipr/graphics/graphics_characters.h"
#include "kipr/graphics/graphics_key_code.h"
#endif

#ifdef KIPR_MODULE_GYRO
#include "kipr/gyro/gyro.hpp"
#endif

#ifdef KIPR_MODULE_MAGNETO
#include "kipr/magneto/magneto.hpp"
#endif

#ifdef KIPR_MODULE_MOTOR
#include "kipr/motor/motor.hpp"
#endif

#ifdef KIPR_MODULE_SERVO
#include "kipr/servo/servo.hpp"
#endif

#ifdef KIPR_MODULE_THREAD
#include "kipr/thread/thread.hpp"
#endif

#ifdef KIPR_MODULE_TIME
#include "kipr/time/time.h"
#endif

#ifdef KIPR_MODULE_WAIT_FOR
#include "kipr/wait_for/wait_for.h"
#endif

#include <stdio.h>
#include <math.h>

#endif


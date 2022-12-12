/*!
 * \file console.h
 * \brief Methods for working with the console screen
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup console Console
 */
/**
 * \page console
 * The functions defined here are functions
 * relating to the console, or output window, of the
 * UI.
 * \ingroup console
 */

#ifndef _KIPR_CONSOLE_CONSOLE_H_
#define _KIPR_CONSOLE_CONSOLE_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Clears the UI's console.
 * \ingroup console
 */
EXPORT_SYM void console_clear();

#ifdef __cplusplus
}
#endif

#endif

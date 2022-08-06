/*!
 * \file console.h
 * \brief Methods for working with the console screen
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup console Console
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

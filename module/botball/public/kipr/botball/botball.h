#ifndef _KIPR_BOTBALL_BOTBALL_H_
#define _KIPR_BOTBALL_BOTBALL_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Register an event to stop the user program after s seconds
 * \param[in] s The number of seconds to wait before the end of program
 */
EXPORT_SYM void shut_down_in(double s);

/*!
 * Initiate a calibration and wait for light sequence
 * which is a normal early step in Botball programs
 * \param[in] light_port_ The analog port and light sensor to use.
 */
EXPORT_SYM void wait_for_light(int light_port_);

#ifdef __cplusplus
}
#endif

#endif
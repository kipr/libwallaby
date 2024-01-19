#ifndef _KIPR_WAIT_FOR_WAIT_FOR_H_
#define _KIPR_WAIT_FOR_WAIT_FOR_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Waits (blocks) for the given number of milliseconds.
 * \param msecs the number of milliseconds to sleep
 */
EXPORT_SYM void wait_for_milliseconds(long msecs);

/**
 * Waits (blocks) until the digital sensor on the given port has been activated.
 * \param port The port to wait for activation on
 */
EXPORT_SYM void wait_for_touch(int port);

/**
 * Waits (blocks) until the a button has been pressed.
 */
EXPORT_SYM void wait_for_a_button();

/**
 * Waits (blocks) until the b button has been pressed.
 */
EXPORT_SYM void wait_for_b_button();

/**
 * Waits (blocks) until the c button has been pressed.
 */
EXPORT_SYM void wait_for_c_button();

/**
 * Waits (blocks) until the x button has been pressed.
 */
EXPORT_SYM void wait_for_x_button();

/**
 * Waits (blocks) until the y button has been pressed.
 */
EXPORT_SYM void wait_for_y_button();

/**
 * Waits (blocks) until the z button has been pressed.
 */
EXPORT_SYM void wait_for_z_button();

/**
 * Waits (blocks) until the side button has been pressed.
 */
EXPORT_SYM void wait_for_side_button();

/**
 * Waits (blocks) until any system button has been pressed.
 */
EXPORT_SYM void wait_for_any_button();

/**
 * Waits (blocks) until the a button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_a_button_clicked();

/**
 * Waits (blocks) until the b button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_b_button_clicked();

/**
 * Waits (blocks) until the c button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_c_button_clicked();

/**
 * Waits (blocks) until the x button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_x_button_clicked();

/**
 * Waits (blocks) until the y button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_y_button_clicked();

/**
 * Waits (blocks) until the z button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_z_button_clicked();

/**
 * Waits (blocks) until the side button has been pressed and then subsequently released.
 */
EXPORT_SYM void wait_for_side_button_clicked();

#ifdef __cplusplus
}
#endif

#endif

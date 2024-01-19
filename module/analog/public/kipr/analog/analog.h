/*!
 * \file analog.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup analog Analogs
 */
/**
 * \page Analog
 * The functions defined here allow for reading the values
 * in analog sensors.
 * \section example_analog_distance Example
 * For example, if there was a distance sensor
 * in analog port 0, then you could use the following code block
 * to move until close to an object or wall
 * ```
 * #include <kipr/wombat.h>
 * #include <stdio.h>
 *
 * int main(){
 *      // arbitrary number that represents how close to get to the wall/object
 *      // larger numbers mean closer to the wall/object
 *      int stopping_distance = 1200;
 *
 *      while (analog(0) < stopping_distance){
 *          // move
 *      }
 * }
 * ```
 * \ingroup analog
 */

#ifndef _KIPR_ANALOG_ANALOG_H_
#define _KIPR_ANALOG_ANALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the 12-bit analog value of a port.
 * \param[in] port A value between 0 and 5 specifying the sensor to read from.
 * \return The latest 12-bit value of the port (a value in the range 0 to 4095).
 * \see analog12
 * \ingroup analog
 */
int analog(int port);


/*!
 * Gets the 8-bit analog value of a port.
 * \param[in] port A value between 0 and 5 specifying the sensor to read from.
 * \return The latest 8-bit value of the port (a value in the range 0 to 255).
 * \see analog
 * \ingroup analog
 */
int analog8(int port);


/*!
 * Gets the 10-bit analog value of a port.
 * \param[in] port A value between 0 and 5 specifying the sensor to read from.
 * \return The latest 10-bit value of the port (a value in the range 0 to 1023).
 * \see analog
 * \ingroup analog
 */
int analog10(int port);


/*!
 * Gets the 12-bit analog value of a port.
 * \param[in] port A value between 0 and 5 specifying the sensor to read from.
 * \return The latest 12-bit value of the port (a value in the range 0 to 4095).
 * \see analog
 * \ingroup analog
 */
int analog12(int port);


/*!
 * Gets the 10-bit analog value of an ET sensor on the given port.
 * \param[in] port A value between 0 and 7 specifying the ET sensor to read from.
 * \return The latest 10-bit value of the port (a value in the range 0 to 1023).
 * \see analog
 * \ingroup analog
 * \deprecated defaulting to analog() on the Wallaby
 */
int analog_et(int port);


/*!
 * Sets analog pullup status for one port.
 * \param[in] port A value between 0 and 5 specifying the analog sensor to read from.
 * \param[in] pullup A value of 0 (inactive) or 1 (active).
 * \see analog
 * \ingroup analog
 * \deprecated no effect on the Wallaby
 */

void set_analog_pullup(int port, int pullup);

/*!
 * Gets the analog pullup status for one portt.
 * \param[in] port A value between 0 and 7 specifying the analog sensor to read from.
 * \return The status of the analog pullup on the specified port
 * \see analog
 * \ingroup analog
 * \deprecated no effect on the Wallaby
 */
int get_analog_pullup(int port);

#ifdef __cplusplus
}
#endif

#endif
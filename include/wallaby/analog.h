/*
 * analog.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ANALOG_H_
#define INCLUDE_WALLABY_ANALOG_H_

/*!
 * \file analog.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup analog Analogs
 */


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
 * \ingroup sensor
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


#endif /* INCLUDE_WALLABY_ANALOG_H_ */

/*!
 * \file accel.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup accel Accelerometer
 */
/**
 * \page Accelerometer
 * The functions here allow for reading the accelerometer.
 * The accelerometer measures acceleration in the x, y, and z directions.
 * \ingroup accel
 */

#ifndef _KIPR_ACCEL_ACCEL_H_
#define _KIPR_ACCEL_ACCEL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Gets the sensed x acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wombat: +/- 2G range,  1024 per G
 * \description This should be approximately 0 when at rest and flat on a table
 * \return The latest signed x acceleration value
 * \ingroup accel
 */
signed short accel_x();


/*!
 * \brief Gets the sensed y acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wombat: +/- 2G range,  1024 per G
 * \description This should be approximately 0 when at rest and flat on a table
 * \return The latest signed y acceleration value
 * \ingroup accel
 */
signed short accel_y();


/*!
 * \brief Gets the sensed z acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wallaby: This should be approximately -1024 when at rest and flat on a table
 * \description Wombat: +/- 2G range,  1024 per G
 * \description Wombat: This should be approximately -1024 when at rest and flat on a table
 * \return The latest signed z acceleration value
 * \ingroup accel
 */
signed short accel_z();


/*!
 * \brief Calibrates the accelerometer by measuring and removing its at-rest offset
 * \description Blocks for about 500 ms while it averages readings of the still controller, then subtracts that offset from every later accel_x(), accel_y() and accel_z() reading. It does not filter or smooth the readings.
 * \description The controller must be still while calibrating, resting flat, upside down or on any side. Gravity is taken to be along the axis that reads largest: that axis keeps +/-1 g (about 1024) and the other two read about 0. Any tilt away from that axis is treated as offset, so later readings are relative to the calibration position.
 * \description Call it at the start of your program, or before you use the accelerometer, if you want corrected readings. Until then readings are uncorrected. The offset lasts until the program exits or accel_calibrate() is called again.
 * \description Fails, keeping any previous calibration, if the controller moves while sampling or the total reading is too far from 1 g to be gravity.
 * \note Returns 1 on success, unlike gyro_calibrate(), which returns 0 on success.
 * \return 1: success 0: failure
 * \ingroup accel
 */
int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif

/*!
 * \file gyro.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup gyro Gyrometer
 */

#ifndef _KIPR_GYRO_GYRO_H_
#define _KIPR_GYRO_GYRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the sensed x rotation
 * \return The latest signed x rotation value
 * \description Wallaby: +/- 250 degree/sec range, 1024 per 250 degrees
 * \description Wombat: +/- 250 degree/sec range, 512 per 250 degrees
 * \ingroup gyro
 */
signed short gyro_x();

/*!
 * Gets the sensed y rotation
 * \return The latest signed y rotation value
 * \description Wallaby: +/- 250 degree/sec range, 1024 per 250 degrees
 * \description Wombat: +/- 250 degree/sec range, 512 per 250 degrees
 * \ingroup gyro
 */
signed short gyro_y();

/*!
 * Gets the sensed z rotation
 * \return The latest signed z rotation value
 * \description Wallaby: +/- 250 degree/sec range, 1024 per 250 degrees
 * \description Wombat: +/- 250 degree/sec range, 512 per 250 degrees
 * \ingroup gyro
 */
signed short gyro_z();

/*!
 * \brief Calibrates the gyroscope by measuring and removing its zero-rate offset
 * \description Blocks for about 500 ms while it averages readings of the still controller, then subtracts that offset from every later gyro_x(), gyro_y() and gyro_z() reading, so a still controller reads about 0. It does not filter or smooth the readings.
 * \description Call it at the start of your program, or before you use the gyroscope, if you want corrected readings. Until then readings are uncorrected. The offset lasts until the program exits or gyro_calibrate() is called again.
 * \description Fails, keeping any previous calibration, if the controller moves while sampling.
 * \note Returns 0 on success, unlike accel_calibrate(), which returns 1 on success.
 * \return 0: success -1: failure
 * \ingroup gyro
 */
int gyro_calibrate();

#ifdef __cplusplus
}
#endif


#endif

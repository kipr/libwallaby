/*
 * gyro.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GYRO_H_
#define INCLUDE_WALLABY_GYRO_H_

/*!
 * \file gyro.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup gyro Gyrometer
 */


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
 * \description Calibrates gyroscope
 * \description Sets a low-pass filter. Put at beginning of your program or before you use gyroscope commands if you want calibrated output.
 * \ingroup gyro
 */
int gyro_calibrate();

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_GYRO_H_ */

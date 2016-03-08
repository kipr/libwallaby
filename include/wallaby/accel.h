/*
 * accel.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ACCEL_H_
#define INCLUDE_WALLABY_ACCEL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the sensed x acceleration
 * \return The latest signed x acceleration value
 */
signed short accel_x();


/*!
 * Gets the sensed x acceleration
 * \return The latest signed y acceleration value
 */
signed short accel_y();


/*!
 * Gets the sensed x acceleration
 * \return The latest signed z acceleration value
 */
signed short accel_z();


/*!
 * Initiates a calibration of the accelerometer
 * \note Not Yet Implemented
 * \return 1: success 0: failure
 */
int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_ACCEL_H_ */

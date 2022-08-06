/*
 * accel.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ACCEL_H_
#define INCLUDE_WALLABY_ACCEL_H_

/*!
 * \file analog.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup accel Accelerometer
 */

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
 * \description Wombat: This should be approximately -512 when at rest and flat on a table
 * \return The latest signed z acceleration value
 * \ingroup accel
 */
signed short accel_z();


/*!
 * Initiates a calibration of the accelerometer
 * \note Not Yet Implemented
 * \return 1: success 0: failure
 * \ingroup accel
 */
int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_ACCEL_H_ */

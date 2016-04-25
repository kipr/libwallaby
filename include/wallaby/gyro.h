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
 * \ingroup gyro
 */
signed short gyro_x();

/*!
 * Gets the sensed y rotation
 * \return The latest signed y rotation value
 * \ingroup gyro
 */
signed short gyro_y();

/*!
 * Gets the sensed z rotation
 * \return The latest signed z rotation value
 * \ingroup gyro
 */
signed short gyro_z();

/*!
 * Initiates a calibration of the gyrometer
 * \note Not Yet Implemented
 * \return 1: success 0: failure
 * \ingroup gyro
 */
int gyro_calibrate();

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_GYRO_H_ */

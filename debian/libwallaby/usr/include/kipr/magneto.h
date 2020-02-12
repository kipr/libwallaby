/*
 * magneto.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MAGNETO_H_
#define INCLUDE_WALLABY_MAGNETO_H_

/*!
 * \file magneto.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup magneto Magnetometer
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the sensed x magneto value
 * \return The latest signed x magneto value
 * \ingroup magneto
 */
signed short magneto_x();

/*!
 * Gets the sensed x magneto value
 * \return The latest signed y magneto value
 * \ingroup magneto
 */
signed short magneto_y();

/*!
 * Gets the sensed x magneto value
 * \return The latest signed z magneto value
 * \ingroup magneto
 */
signed short magneto_z();

/*!
 * Initiates a calibration of the magnetometer
 * \note Not Yet Implemented
 * \return 1: success 0: failure
 * \ingroup magneto
 */
int magneto_calibrate();

#ifdef __cplusplus
}
#endif




#endif /* INCLUDE_WALLABY_MAGNETO_H_ */

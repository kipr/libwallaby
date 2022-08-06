/*!
 * \file magneto.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup magneto Magnetometer
 */

#ifndef _KIPR_MAGNETO_MAGNETO_H_
#define _KIPR_MAGNETO_MAGNETO_H_

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

#endif
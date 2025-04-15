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
 * \brief Gets the sensed x magnetometer value in micro Tesla (μT) in the NED frame (Side with ports is defined as North)
 * \description Measures the magnetic field strength along the x-axis in the NED reference frame
 * \description Wombat: Used for compass and magnetic field detection
 * \return The latest signed x magnetometer value in μT
 * \ingroup magneto
 */
signed short magneto_x();

/*!
 * \brief Gets the sensed y magnetometer value in micro Tesla (μT) in the NED frame (Side with ports is defined as North)
 * \description Measures the magnetic field strength along the y-axis in the NED reference frame
 * \description Wombat: Used for compass and magnetic field detection
 * \return The latest signed y magnetometer value in μT
 * \ingroup magneto
 */
signed short magneto_y();

/*!
 * \brief Gets the sensed z magnetometer value in micro Tesla (μT) in the NED frame (Side with ports is defined as North)
 * \description Measures the magnetic field strength along the z-axis in the NED reference frame
 * \description Wombat: Used for compass and magnetic field detection
 * \return The latest signed z magnetometer value in μT
 * \ingroup magneto
 */
signed short magneto_z();

#ifdef __cplusplus
}
#endif

#endif

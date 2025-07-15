/*!
 * \file gyro.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup gyro Gyrometer
 * \authors Tobias Madlberger <tobias.madlberger@gmail.com>
 */

#ifndef _KIPR_GYRO_GYRO_H_
#define _KIPR_GYRO_GYRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Gets the sensed x rotation in degrees per second (deg/s) in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2000 deg/s range
 * \description Measures rotation around the x-axis in the NED reference frame
 * \return The latest signed x rotation value in deg/s
 * \ingroup gyro
 */
signed short gyro_x();

/*!
 * \brief Gets the sensed y rotation in degrees per second (deg/s) in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2000 deg/s range
 * \description Measures rotation around the y-axis in the NED reference frame
 * \return The latest signed y rotation value in deg/s
 * \ingroup gyro
 */
signed short gyro_y();

/*!
 * \brief Gets the sensed z rotation in degrees per second (deg/s) in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2000 deg/s range
 * \description Measures rotation around the z-axis in the NED reference frame
 * \return The latest signed z rotation value in deg/s
 * \ingroup gyro
 */
signed short gyro_z();

/*!
 * \brief Calibrates gyroscope
 * \description Sets a low-pass filter. Put at beginning of your program or before you use gyroscope commands if you want calibrated output.
 * \description This function will block for around 500ms taking samples of the gyroscope at standstill.
 * \return 1: success 0: failure
 * \ingroup gyro
 */
int gyro_calibrate();

#ifdef __cplusplus
}
#endif


#endif

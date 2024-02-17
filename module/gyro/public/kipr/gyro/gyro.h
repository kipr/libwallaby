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
 * \description Calibrates gyroscope
 * \description Sets a low-pass filter. Put at beginning of your program or before you use gyroscope commands if you want calibrated output.
 * \description This function will block for around 500ms taking samples of the gyroscope at standstill.
 * \ingroup gyro
 */
int gyro_calibrate();

#ifdef __cplusplus
}
#endif


#endif

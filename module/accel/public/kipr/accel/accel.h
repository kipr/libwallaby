/*!
 * \file analog.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup accel Accelerometer
 */
/**
 * \page Accelerometer
 * The functions here allow for reading the accelerometer.
 * The accelerometer measures acceleration in the x, y, and z directions.
 * \ingroup accel
 */

#ifndef _KIPR_ACCEL_ACCEL_H_
#define _KIPR_ACCEL_ACCEL_H_

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
 * \brief Calibrates a lowpass filter for the accelerometer
 * \description Sets a low-pass filter. Put at beginning of your program or before you use accelerometer commands if you want calibrated output.
 * \description This function will block for around 500ms taking samples of the accelerometer at standstill.
 * \return 1: success 0: failure
 * \ingroup accel
 */
int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif

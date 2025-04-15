/*!
 * \file accel.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup accel Accelerometer
 * \authors Tobias Madlberger <tobias.madlberger@gmail.com>
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
 * \brief Gets the sensed x acceleration in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2G range
 * \description This should be approximately 0 when at rest and flat on a table.
 * The measured unit is in m/s²
 * \return The latest signed x acceleration value
 * \ingroup accel
 */
signed short accel_x();


/*!
 * \brief Gets the sensed y acceleration in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2G range
 * \description This should be approximately 0 when at rest and flat on a table
 * The measured unit is in m/s²
 * \return The latest signed y acceleration value
 * \ingroup accel
 */
signed short accel_y();


/*!
 * \brief Gets the sensed z acceleration in the NED frame (Side with ports is defined as North)
 * \description Wombat: +/- 2G range
 * \description Wombat: This should be approximately -9.8 when at rest and flat on a table
 * The measured unit is in m/s²
 * \return The latest signed z acceleration value
 * \ingroup accel
 */
signed short accel_z();


/*!
 * \brief Calibrates a lowpass filter for the accelerometer
 * \description Sets a low-pass filter. Put at beginning of your program or before you use accelerometer commands if you want calibrated output.
 * \description This function will block for around 500ms taking samples of the accelerometer at standstill.
 * \description This function will determine the accelerometer bias as well as the gravity direction.
 * \description When the wombat is flat at rest, the reading should be calibrated to be approximately 0, 0, -9.8.
 * \return 1: success 0: failure
 * \ingroup accel
 */
int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif


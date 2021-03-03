#ifndef ACCEL_H_
#define ACCEL_H

#include "sensor.hpp"

/*!
 * \file analog.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup accel Accelerometer
 */

#ifdef __cplusplus
extern "C" {
#endif

namespace Private
{

/*!
 * \brief Gets the sensed x acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wombat: +/- 2G range,  1024 per G
 * \description This should be approximately 0 when at rest and flat on a table
 * \return The latest signed x acceleration value
 * \ingroup accel
 */
short accel_x(unsigned char * alt_read_buffer = nullptr);

/*!
 * \brief Gets the sensed y acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wombat: +/- 2G range,  1024 per G
 * \description This should be approximately 0 when at rest and flat on a table
 * \return The latest signed y acceleration value
 * \ingroup accel
 */
short accel_y(unsigned char * alt_read_buffer = nullptr);

/*!
 * \brief Gets the sensed z acceleration
 * \description Wallaby: +/- 2G range,  1024 per G
 * \description Wallaby: This should be approximately -1024 when at rest and flat on a table
 * \description Wombat: +/- 2G range,  1024 per G
 * \description Wombat: This should be approximately -512 when at rest and flat on a table
 * \return The latest signed z acceleration value
 * \ingroup accel
 */
short accel_z(unsigned char * alt_read_buffer = nullptr);

/*!
 * Initiates a calibration of the accelerometer
 * \note Not Yet Implemented
 * \return 1: success 0: failure
 * \ingroup accel
 */
bool accel_calibrate();



}

#ifdef __cplusplus
}
#endif

#endif /* SRC_ACCEL_H_ */
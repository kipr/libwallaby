/*
 * accel.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ACCEL_H_
#define INCLUDE_WALLABY_ACCEL_H_

#ifdef __cplusplus
extern "C" {
#endif

signed short accel_x();

signed short accel_y();

signed short accel_z();

int accel_calibrate();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_ACCEL_H_ */

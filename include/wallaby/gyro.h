/*
 * gyro.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GYRO_H_
#define INCLUDE_WALLABY_GYRO_H_


#ifdef __cplusplus
extern "C" {
#endif

signed short gyro_x();

signed short gyro_y();

signed short gyro_z();

int gyro_calibrate();

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_GYRO_H_ */

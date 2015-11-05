/*
 * magneto.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MAGNETO_H_
#define INCLUDE_WALLABY_MAGNETO_H_



#ifdef __cplusplus
extern "C" {
#endif

signed short magneto_x();

signed short magneto_y();

signed short magneto_z();

int magneto_calibrate();

#ifdef __cplusplus
}
#endif




#endif /* INCLUDE_WALLABY_MAGNETO_H_ */

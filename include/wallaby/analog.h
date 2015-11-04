/*
 * analog.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ANALOG_H_
#define INCLUDE_WALLABY_ANALOG_H_


#ifdef __cplusplus
extern "C" {
#endif


int analog(int port);

int analog8(int port);

int analog10(int port);

int analog12(int port);

int analog_et(int port);

void set_analog_pullup(int port, int pullup);

int get_analog_pullup(int port);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_ANALOG_H_ */

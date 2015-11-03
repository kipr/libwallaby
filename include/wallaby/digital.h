/*
 * digital.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_DIGITAL_H_
#define INCLUDE_WALLABY_DIGITAL_H_

#ifdef __cplusplus
extern "C" {
#endif


int digital(int port);

void set_digital_value(int port, int value);

int get_digital_value(int port);

void set_digital_output(int port, int out);

int get_digital_output(int port);

int get_digital_pullup(int port);

void set_digital_pullup(int port, int pullup);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_DIGITAL_H_ */

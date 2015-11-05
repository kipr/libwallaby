/*
 * servo.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_SERVO_H_
#define INCLUDE_WALLABY_SERVO_H_


#ifdef __cplusplus
extern "C" {
#endif

void enable_servo(int port);

void disable_servo(int port);

void enable_servos();

void disable_servos();

void set_servo_enabled(int port, int enabled);

int get_servo_enabled(int port);

int get_servo_position(int port);

void set_servo_position(int port, int position);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_SERVO_H_ */

/*
 * motors.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MOTORS_H_
#define INCLUDE_WALLABY_MOTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

int get_motor_position_counter(int motor);

int gmpc(int motor);

void clear_motor_position_counter(int motor);

void cmpc(int motor);

int move_at_velocity(int motor, int velocity);

int mav(int motor, int velocity);

int move_to_position(int motor, int speed, int goal_pos);

int mtp(int motor, int speed, int goal_pos);

int move_relative_position(int motor, int speed, int delta_pos);

int mrp(int motor, int speed, int delta_pos);

void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd);

void get_pid_gains(int motor, short * p, short * i, short * d, short * pd, short * id, short * dd);

int freeze(int motor);

int get_motor_done(int motor);

void block_motor_done(int motor);

void bmd(int motor);

int setpwm(int motor, int pwm);

int getpwm(int motor);

void fd(int motor);

void bk(int motor);

void motor(int motor, int percent);

void off(int motor);

void alloff();

void ao();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_MOTORS_H_ */

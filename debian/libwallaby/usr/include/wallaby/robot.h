/*
 * robot.h
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ROBOT_H_
#define INCLUDE_WALLABY_ROBOT_H_


#ifdef __cplusplus
extern "C" {
#endif



unsigned long int get_robot_states_sequence_num();
unsigned long int get_robot_update_count();

unsigned short get_robot_firmware_version();

int set_robot_update_delay(unsigned int us_delay);
unsigned int get_robot_update_delay();

int set_low_volt_threshold(float volts);
float get_low_volt_threshold();

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_ROBOT_H_ */

/*
 * battery.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_BATTERY_H_
#define INCLUDE_WALLABY_BATTERY_H_


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * Whether or not the battery is charging
 * \return 0: not charging  1: charging
 */
int battery_charging();


/*!
 * The device's power level
 * \return The device's current voltage
 */
float power_level();

float power_level_nimh();

float power_level_lipo();

float power_level_life();



#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_BATTERY_H_ */

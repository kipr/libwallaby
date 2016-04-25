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
 * \file battery.h
 * \author Joshua southerland
 * \copyright KISS Institute for Practical Robotics
 * \defgroup battery Battery
 */


/*!
 * Whether or not the battery is charging
 * \return 0: not charging  1: charging
 * \deprecated not applicable for the Wallaby controller
 * \ingroup battery
 */
int battery_charging();


/*!
 * The device's power level
 * \return The device's current battery capacity 0: 0%   1: 100%
 * \note defaults to LiFePO4 chemistry
 * \ingroup battery
 */
float power_level();

/*!
 * The device's power level (NiMH chemistry)
 * \return The device's current battery capacity 0: 0%   1: 100%
 * \ingroup battery
 */
float power_level_nimh();

/*!
 * The device's power level (LiPo chemistry)
 * \return The device's current battery capacity 0: 0%   1: 100%
 * \ingroup battery
 */
float power_level_lipo();

/*!
 * The device's power level (LiFePO4 chemistry)
 * \return The device's current battery capacity 0: 0%   1: 100%
 * \see power_level()
 * \ingroup battery
 */
float power_level_life();



#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_BATTERY_H_ */

/*
 * servo.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_SERVO_H_
#define INCLUDE_WALLABY_SERVO_H_


/*!
 * \file servo.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup servo Servos
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enable a specific servo
 * \param[in] port The port, between 0 and 3, to enable
 * \ingroup servo
 */
void enable_servo(int port);

/**
 * \brief Disable a specific servo
 * \param[in] port The port, between 0 and 3, to disable
 * \ingroup servo
 */
void disable_servo(int port);

/**
 * \brief Enable all four servo channels
 * \ingroup servo
 */
void enable_servos();

/**
 * \brief Disable all four servo channels
 * \ingroup servo
 */
void disable_servos();

/**
 * \brief Enable or disable a specific servo
 * \param[in] port The port, between 0 and 3, to enable
 * \param[in] enabled The new enable setting 0: disabled 1: enabled
 * \ingroup servo
 */
void set_servo_enabled(int port, int enabled);

/**
 * \brief Check if a servo is enabled
 * \param[in] port The port, between 0 and 3
 * \return The servo enable setting 0: disabled 1: enabled
 * \ingroup servo
 */
int get_servo_enabled(int port);

/*!
 * \brief Get the most recent commanded servo position
 * \param servo The port of the servo
 * \return The servo's position as an 11 bit integer (which is an integer between 0 and 2047)
 * \note This method will return the last _sent_ position, not the currently _set_ position.
 * \ingroup servo
 */
int get_servo_position(int port);

/*!
 * \brief Set a new servo goal position
 * \param servo The port of the servo
 * \param position The new servo position, between 0 and 2047
 *
 * \note Even though the servos have a _theoretical_ range between 0 and 2047,
 * the _actual_ range is often less. Setting the servo to a position that it cannot physically
 * reach will cause the servo to audibly strain and will consume battery very quickly.
 * \ingroup servo
 */
void set_servo_position(int port, int position);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_SERVO_H_ */

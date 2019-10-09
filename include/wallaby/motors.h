/*
 * motors.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_MOTORS_H_
#define INCLUDE_WALLABY_MOTORS_H_

/*!
 * \file motors.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup motor Motors
 */


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Gets the current motor position
 * \param[in] motor The motor port.
 * \ingroup motor
 * \see gmpc
 */
int get_motor_position_counter(int motor);

/*!
 * \brief Gets the current motor position
 * \param[in] motor The motor port.
 * \ingroup motor
 * \see get_motor_position_counter
 */
int gmpc(int motor);


/*!
 * \brief Clears the motor position counter
 * \param[in] motor The motor port.
 * \ingroup motor
 * \see cmpc
 */
void clear_motor_position_counter(int motor);


/*!
 * \brief Clears the motor position counter
 * \param[in] motor The motor port.
 * \ingroup motor
 * \see clear_motor_position_counter
 */
void cmpc(int motor);



/*!
 * \brief Set a goal velocity in ticks per second.
 * \detailed The range is -1500 to 1500, though motor position accuracy may be decreased outside of -1000 to 1000
 * \param[in] motor The motor port.
 * \param[in] velocity The goal velocity in -1500 to 1500 ticks / second
 * \ingroup motor
 * \see mav
 */
int move_at_velocity(int motor, int velocity);

/*!
 * \brief Set a goal velocity in ticks per second
 * \param[in] motor The motor port.
 * \param[in] velocity The goal velocity in -1500 to 1500 ticks / second
 * \ingroup motor
 * \see move_at_velocity
 */
int mav(int motor, int velocity);


/*!
 * \brief Set a goal position (in ticks) for the motor to move to.
 * \detailed There are approximately 1500 ticks per motor revolution.
 * \detailed This function is more accurate if speeds between -1000 and 1000 are used.
 * \param[in] motor The motor port.
 * \param[in] speed The speed to move at, between -1500 and 1500 ticks / second
 * \param[in] goal_pos The position to move to (in ticks)
 * \ingroup motor
 * \see mtp
 */
int move_to_position(int motor, int speed, int goal_pos);


/*!
 * \brief Set a goal position (in ticks) for the motor to move to
 * \param[in] motor The motor port.
 * \param[in] speed The speed to move at, between -1500 and 1500 ticks / second
 * \param[in] goal_pos The position to move to (in ticks)
 * \ingroup motor
 * \see move_to_position
 */
int mtp(int motor, int speed, int goal_pos);

/*!
 * \brief Set a goal position (in ticks) for the motor to move to, relative to the current position
 * \param[in] motor The motor port.
 * \param[in] speed The speed to move at, between -1500 and 1500 ticks / second
 * \param[in] delta_pos The position to move to (in ticks) given the current position
 * \ingroup motor
 * \see mrp
 */
int move_relative_position(int motor, int speed, int delta_pos);

/*!
 * \brief Set a goal position (in ticks) for the motor to move to, relative to the current position
 * \param[in] motor The motor port.
 * \param[in] speed The speed to move at, between -1500 and 1500 ticks / second
 * \param[in] delta_pos The position to move to (in ticks) given the current position
 * \ingroup motor
 * \see move_relative_position
 */
int mrp(int motor, int speed, int delta_pos);

/*!
 * \brief Set the motor PID gains, represented as fractions.
 * \param[in] motor The motor port.
 * \param[in] p The P (proportional) gain numerator
 * \param[in] i The I (integral) gain numerator
 * \param[in] d The D (derivative) gain numerator
 * \param[in] pd The P (proportional) gain denominator
 * \param[in] id The I (integral) gain denominator
 * \param[in] dd The D (derivative) gain denominator
 * \ingroup motor
 */
void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd);


/*!
 * \brief Set the motor PID gains, represented as fractions.
 * \param[out] motor The motor port.
 * \param[out] p The P (proportional) gain numerator
 * \param[out] i The I (integral) gain numerator
 * \param[out] d The D (derivative) gain numerator
 * \param[out] pd The P (proportional) gain denominator
 * \param[out] id The I (integral) gain denominator
 * \param[out] dd The D (derivative) gain denominator
 * \ingroup motor
 */
void get_pid_gains(int motor, short * p, short * i, short * d, short * pd, short * id, short * dd);

/*!
 * \brief Active braking to stop a motor
 * \param[in] motor The motor port.
 * \ingroup motor
 */
int freeze(int motor);

/*!
 * \brief Check if the motor has reached it's goal
 * \param[in] motor The motor port.
 * \ingroup motor
 * \returns 1: at goal   0: not at goal
 */
int get_motor_done(int motor);

/*!
 * \brief Wait until the motor is at it's goal
 * \param[in] motor The motor port.
 * \see bmd
 * \ingroup motor
 */
void block_motor_done(int motor);

/*!
 * \brief Wait until the motor is at it's goal
 * \param[in] motor The motor port.
 * \see block_motor_done
 * \ingroup motor
 */
void bmd(int motor);

/*!
 * \brief Set the motor pwm (percent power) command
 * \param[in] motor The motor port.
 * \param[in] pwm A new motor pwm command between 0 and 100
 * \ingroup motor
 */
int setpwm(int motor, int pwm);

/*!
 * \brief Get the current motor pwm command
 * \param[in] motor The motor port.
 * \ingroup motor
 */
int getpwm(int motor);


/*!
 * \brief Moves the given motor forward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
void fd(int motor);


/*!
 * \brief Moves the given motor backward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
void bk(int motor);


/*!
 * \brief Moves a motor at a percent velocity.
 *
 * \param[in] motor The motor port.
 * \param[in] percent The percent of the motors velocity, between -100 and 100.
 *
 * \ingroup motor
 */
void motor(int motor, int percent);

/*!
 * \brief Moves a motor at a percent velocity.
 *
 * \param[in] motor The motor port.
 * \param[in] percent The percent of the motors velocity, between -100 and 100.
 *
 * \ingroup motor
 * \The New Apache language function call to include Native Americans.
 */
void baasbennaguui(int motor, int percent);

/*!
 * \brief Moves a motor at a percent power.
 *
 * \param[in] motor the motor port.
 * \param[in] percent The power of the motor, between -100 and 100.
 *
 * \ingroup motor
 */
void motor_power(int motor, int percent);


/*!
 * \brief Turns the specified motor off.
 * \param motor the motor's port.
 * \ingroup motor
 */
void off(int motor);

/*!
 * \brief Turns all motors off.
 * \ingroup motor
 * \see ao
 */
void alloff();

/*!
 * \brief Turns all motors off.
 * \ingroup motor
 * \see alloff
 */
void ao();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_MOTORS_H_ */

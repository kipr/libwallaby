/*
 * create.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_CREATE_H_
#define INCLUDE_WALLABY_CREATE_H_

/*!
 * \file create.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup create iRobot (R) Create (TM)
 */

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*!
   * Waits to establish a connection to the create.
   * \blocksuntil A connection to a create is established
   * \return 0 on success
   * \see create_disconnect
   * \ingroup create
   */
  VF EXPORT_SYM int create_connect();

  /*!
   * Attempts to establish a connection to the create.
   * \return 1 if connection succeeded, 0 if connection failed
   * \see create_disconnect
   * \ingroup create
   */
  VF EXPORT_SYM int create_connect_once();

  /*!
   * Cleans up connection to the create.
   * \see create_connect
   * \ingroup create
   */
  VF EXPORT_SYM void create_disconnect();

  /*!
   * Sets the create mode to passive. This means
   * that you WILL NOT have control of the roomba.
   * You will be able to get sensor data, but trying
   * to control the roomba will not work.
   * \ingroup create
   */
  VF EXPORT_SYM void create_passive();

  /*!
   * Sets the create mode to safe. This means
   * that the create will automatically stop its wheels
   * once any of them lose contact with the ground
   * \ingroup create
   */
  VF EXPORT_SYM void create_safe();

  /*!
   * Sets the create mode to full. This means that the create
   * WILL NOT automatically stop its wheels once they lose
   * contact with the ground.
   * \ingroup create
   */
  VF EXPORT_SYM void create_full();

  /*!
   * Starts spot cleaning mode on the roomba.
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_spot();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_cover();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_demo(int d);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_cover_dock();

  /*!
   * Returns the create's current connection mode.
   * 0 is off (no sensor data, no control),
   * 1 is passive (sensor data, no control),
   * 2 is safe (sensor data, most control),
   * 3 is full (sensor data, full control).
   * \see create_full create_safe create_passive create_stop
   * \returns int
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_mode();

  /*!
   * Returns whether the create's left bumper
   * is pressed. 1 is pressed, 0 is unpressed.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lbump();

  /*!
   * Returns whether the create's right bumper
   * is pressed. 1 is pressed, 0 is unpressed.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rbump();

  /*!
   * Returns whether the create's left wheel
   * has dropped or not. 1 is dropped, 0 is not
   * dropped. In safe mode, if a wheel has dropped,
   * the create will automatically stop itself.
   * \note a "dropped" wheel is one that is no longer touching
   * the ground (it has 'dropped' as much as it can from the roomba)
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lwdrop();

  /*!
   * Returns if the caster wheel has dropped.
   * 1 is dropped, 0 is not dropped.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_cwdrop();

  /*!
   * Returns whether the create's right wheel
   * has dropped or not. 1 is dropped, 0 is not
   * dropped. In safe mode, if a wheel has dropped,
   * the create will automatically stop itself.
   * \note a "dropped" wheel is one that is no longer touching
   * the ground (it has 'dropped' as much as it can from the roomba)
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rwdrop();

  /*!
   * Returns whether or not the create's wall sensor has a value.
   * 1 means that it has a value, 0 means that it doesn't.
   * DOES NOT return the value of the wall sensor.
   * \see get_create_wall_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_wall();

  /*!
   * Returns whether or not the side left cliff sensor
   * has a value. 1 means that it has a value,
   * 0 means that it doesn't. DOES NOT return
   * the value of the side left cliff sensor.
   * \see get_create_lcliff_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lcliff();

  /*!
   * Returns whether or not the front left cliff sensor
   * has a value. 1 means that it has a value,
   * 0 means that it doesn't. DOES NOT return
   * the value of the front left cliff sensor.
   * \see get_create_lfcliff_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lfcliff();

  /*!
   * Returns whether or not the front right cliff sensor
   * has a value. 1 means that it has a value,
   * 0 means that it doesn't. DOES NOT return
   * the value of the front right sensor.
   * \see get_create_rfcliff_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rfcliff();

  /*!
   * Returns whether or not the side right cliff sensor
   * has a value. 1 means that it has a value,
   * 0 means that it doesn't. DOES NOT return
   * the value of the side right sensor.
   * \see get_create_rcliff_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rcliff();

  /*!
   * Returns whether the left-most lightbump sensor has a
   * value. DOES NOT return the value of the left-most lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_llightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_llightbump();

  /*!
   * Returns whether the left-front lightbump sensor has a
   * value. DOES NOT return the value of the left-front lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_lflightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lflightbump();

  /*!
   * Returns whether the left-center lightbump sensor has a
   * value. DOES NOT return the value of the left-center lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_lclightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lclightbump();

  /*!
   * Returns whether the right-center lightbump sensor has a
   * value. DOES NOT return the value of the right-center lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_rclightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rclightbump();

  /*!
   * Returns whether the right-front lightbump sensor has a
   * value. DOES NOT return the value of the right-front lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_rflightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rflightbump();

  /*!
   * Returns whether the right-most lightbump sensor has a
   * value. DOES NOT return the value of the right-most lightbump sensor.
   * The lightbump sensor is the black, reflective strip that runs along
   * the roomba's bumpers.
   * \see get_create_rlightbump_amt
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rlightbump();

  /*!
   * Returns the value of the left-most lightbump sensor. Higher
   * values mean that the left-most lightbump sensor is closer to
   * an object; lower values mean that the left-most lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_llightbump_amt();

  /*!
   * Returns the value of the right-most lightbump sensor. Higher
   * values mean that the right-most lightbump sensor is closer to
   * an object; lower values mean that the right-most lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rlightbump_amt();

  /*!
   * Returns the value of the left-front lightbump sensor. Higher
   * values mean that the left-front lightbump sensor is closer to
   * an object; lower values mean that the left-front lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lflightbump_amt();

  /*!
   * Returns the value of the left-center lightbump sensor. Higher
   * values mean that the left-center lightbump sensor is closer to
   * an object; lower values mean that the left-center lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lclightbump_amt();

  /*!
   * Returns the value of the right-center lightbump sensor. Higher
   * values mean that the right-center lightbump sensor is closer to
   * an object; lower values mean that the right-center lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rclightbump_amt();

  /*!
   * Returns the value of the right-front lightbump sensor. Higher
   * values mean that the right-front lightbump sensor is closer to
   * an object; lower values mean that the right-front lightbump sensor
   * is farther from an object.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rflightbump_amt();

  /*!
   * Returns the whether the create senses a virtual wall.
   * 1 means that it senses a virtual wall, 0 means that it
   * doesn't.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_vwall();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_overcurrents();

  /*!
   * Returns the value returned by the create's infrared
   * sensor.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_infrared();

  /*!
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_advance_button();

  /*!
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_play_button();

  /*!
   * Returns the create's normalized angle. The value
   * will be between 0 and 360.
   * \note positive values indicate clockwise turns; negative values indicate counterclockwise turns
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_normalized_angle();

  /*!
   * Sets what the create refers to its current angle as.
   * This is just a wrapper for set_create_total_angle
   * \see set_create_total_angle
   * \ingroup create
   */
  VF EXPORT_SYM void set_create_normalized_angle(int angle);

  /*!
   * Returns the create's total angle. This is the total
   * amount of degrees that the create has turned and
   * is not necessarily between 0 and 360.
   * \note positive values indicate clockwise turns; negative values indicate counterclockwise turns
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_total_angle();

  /*!
   * Sets what the create refers to its current angle as.
   * \ingroup create
   */
  VF EXPORT_SYM void set_create_total_angle(int angle);

  /*!
   * Returns the distance, in millimeters, that the roomba has traveled.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_distance();

  /*!
   * Sets what the create refers to its current distance as.
   * \ingroup create
   */
  VF EXPORT_SYM void set_create_distance(int dist);

  /*!
   * Returns the create's current battery charging state.
   * 0 means not charging, 1 means reconditioning charging,
   * 2 means full charging, 3 means trickle charging, 4 means
   * waiting, and 5 means charging fault condition.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_charging_state();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_voltage();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_current();

  /*!
   * Returns the temperature of the roomba in degrees Celsius.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_temp();

  /*!
   * Returns the create's current charge in milliamp-hours (mAh).
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_charge();

  /*!
   * Returns the estimated capacity of how many milliamp-hours (mAh)
   * the create's battery can hold.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_battery_capacity();

  /*!
   * Returns the wall signal sensed by the create.
   * \see get_create_wall
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_wall_amt();

  /*!
   * Returns the value measured by the left-most
   * cliff sensor.
   * \note For line following purposes, this can be treated like
   * a tophat sensor value. Lower values often indicate black, higher
   * values often indicate white.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lcliff_amt();

  /*!
   * Returns the value measured by the left-front
   * cliff sensor.
   * \note For line following purposes, this can be treated like
   * a tophat sensor value. Lower values often indicate black, higher
   * values often indicate white.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_lfcliff_amt();

  /*!
   * Returns the value measured by the right-front
   * cliff sensor.
   * \note For line following purposes, this can be treated like
   * a tophat sensor value. Lower values often indicate black, higher
   * values often indicate white.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rfcliff_amt();

  /*!
   * Returns the value measured by the right-most
   * cliff sensor.
   * \note For line following purposes, this can be treated like
   * a tophat sensor value. Lower values often indicate black, higher
   * values often indicate white.
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_rcliff_amt();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_bay_DI();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_bay_AI();

  /*!
   * \return The song slot of the current song
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_song_number();

  /*!
   * \return 1 if it the create is playing a song,
   * 0 if the create isn't playing a song
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_song_playing();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_number_of_stream_packets();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_requested_velocity();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_requested_radius();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_requested_right_velocity();

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int get_create_requested_left_velocity();

  /*!
   * Sets the create connection mode to off. No sensor data
   * will be available, and the create will not respond to
   * movement commands.
   * \ingroup create
   */
  VF EXPORT_SYM void create_stop();

  /*!
   * Drive at the requested speed in an arc with the provided
   * radius.
   * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
   * \param radius The radius (in mm) of the arc to drive through. Range is
   * -2000 to 2000.
   * \ingroup create
   */
  VF EXPORT_SYM void create_drive(int speed, int radius);

  /*!
   * Drive straight at the requested speed.
   * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
   * \ingroup create
   */
  VF EXPORT_SYM void create_drive_straight(int speed);

  /*!
   * Spin clockwise at the requested speed. Spins in place.
   * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
   * \note negative speeds will result in spinning counter-clockwise
   * \ingroup create
   */
  VF EXPORT_SYM void create_spin_CW(int speed);

  /*!
   * Spin counter-clockwise at the requested speed. Spins in place.
   * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
   * \note negative speeds will result in spinning clockwise
   * \ingroup create
   */
  VF EXPORT_SYM void create_spin_CCW(int speed);

  /*!
   * Drive the create at the provided left wheel speed and right wheel speed.
   * \param l_speed The speed (in mm/s) to drive the left wheel at. Range is -500 to 500
   * \param r_speed The speed (in mm/s) to drive the right wheel at. Range is -500 to 500
   * \ingroup create
   */
  VF EXPORT_SYM void create_drive_direct(int l_speed, int r_speed);

  /*!
   * Turns the create the requested number of degrees at the requested speed.
   * \param speed The speed (in mm/s) to turn at. Range is -500 to 500
   * \param angle The angle (in degrees) to turn.
   * \note This is a blocking function, so you DO NOT need an msleep after it.
   * It already sleeps for the time that it needs to execute the turn and
   * doesn't need extra msleeps.
   * \ingroup create
   */
  VF EXPORT_SYM void create_spin_block(int speed, int angle);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM int _create_get_raw_encoders(long *lenc, long *renc);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_advance_led(int on);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_play_led(int on);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_power_led(int color, int brightness);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_digital_output(int bits);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);

  /*!
   * \note Not yet implemented
   * \ingroup create
   */
  VF EXPORT_SYM void create_low_side_drivers(int pwm2, int pwm1, int pwm0);

  /*!
   * Loads a song for playing on the create
   * \param song It should be an array of unsigned chars (positive integers 0-255)
   * The first value in a pair will be the midi value of the note
   * the second value in the pair will be the duration (in 64ths of a second)
   * for example, a song {88, 20, 91, 32, 70, 15} will play midi value 88 for 20/64ths
   * of a second, midi value 91 for 32/64ths of a second, and midi value 70 for
   * 15/64ths of a second.
   * A full list of notes playable on the create is found at
   * https://cdn-shop.adafruit.com/datasheets/create_2_Open_Interface_Spec.pdf on page 34
   * \param length The length of the song. It is how many notes are in the song, not
   * how many items are in your song array.
   * \param num The song slot to load the song into; valid values are 0, 1, 2, and 3
   * \return 1 on success, 0 on failure
   * \note Example use: `unsigned char example_song[] = {88, 20, 91, 32, 70, 15}; create_load_song(example_song, 3, 0);`
   * \ingroup create
   */
  VF EXPORT_SYM int create_load_song(const unsigned char *song, const unsigned char length, const unsigned char num);

  /*!
   * Plays a song that has been loaded. Use create_load_song first.
   * \param num The song slot to play from; valid values are 0, 1, 2, and 3
   * \return 1 on success, 0 on failure
   * \see create_load_song
   * \ingroup create
   */
  VF EXPORT_SYM int create_play_song(const unsigned char num);

  /*!
   * Reads sensor data from the create.
   * \param data - This is where the read sensor data is stored
   * \param count - How many bytes to read from the create.
   * \returns 1 on success, 0 on failure.
   * \ingroup create
   */
  VF EXPORT_SYM int create_read_block(char *data, int count);

  /*!
   * Write a byte to the create. This is used to send commands directly
   * to the create without using the functions provided in this library.
   * \note If you want to directly send bytes to the create, consider checking out
   * the actual create oi specifications: https://cdn-shop.adafruit.com/datasheets/create_2_Open_Interface_Spec.pdf
   * \ingroup create
   */
  VF EXPORT_SYM void create_write_byte(char byte);

  /*!
   * Flushes commands to the create.
   * \note This library already automatically flushes commands to the create
   * \ingroup create
   */
  VF EXPORT_SYM void create_clear_serial_buffer();

  enum BaudRate
  {
    Baud57600 = 0,
    Baud115200 = 1
  };

  /*!
   * Returns the baud rate of the create.
   * \note The baud rate is how many times a second the create updates its sensors and
   * receives commands.
   * \ingroup create
   */
  VF EXPORT_SYM enum BaudRate get_create_baud_rate();

  /*!
   * Sets the create's baud rate to the provided baud rate.
   * \ingroup create
   */
  VF EXPORT_SYM void set_create_baud_rate(const enum BaudRate baudRate);

  VFL

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_CREATE_H_ */

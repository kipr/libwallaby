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
extern "C" {
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
 * \ingroup create
 */
VF EXPORT_SYM void create_start();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_passive();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_safe();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_full();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_spot();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_cover();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_demo(int d);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_cover_dock();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_mode();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lwdrop();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_cwdrop();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rwdrop();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_wall();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lcliff();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lfcliff();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rfcliff();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rcliff();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_llightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lflightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lclightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rclightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rflightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rlightbump();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_llightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rlightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lflightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lclightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rclightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rflightbump_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_vwall();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_overcurrents();

/*!
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
 * \ingroup create
 */
VF EXPORT_SYM int get_create_normalized_angle();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void set_create_normalized_angle(int angle);

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_total_angle();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void set_create_total_angle(int angle);

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_distance();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void set_create_distance(int dist);

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_charging_state();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_voltage();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_current();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_temp();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_charge();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_battery_capacity();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_wall_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lcliff_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_lfcliff_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rfcliff_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_rcliff_amt();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_bay_DI();

/*!
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
 * \ingroup create
 */
VF EXPORT_SYM int get_create_number_of_stream_packets();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_requested_velocity();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_requested_radius();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_requested_right_velocity();

/*!
 * \ingroup create
 */
VF EXPORT_SYM int get_create_requested_left_velocity();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_stop();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_drive(int speed, int radius);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_drive_straight(int speed);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_spin_CW(int speed);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_spin_CCW(int speed);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_drive_direct(int l_speed, int r_speed);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_spin_block(int speed, int angle);

/*!
 * \ingroup create
 */
VF EXPORT_SYM int _create_get_raw_encoders(long *lenc, long *renc);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_advance_led(int on);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_play_led(int on) ;

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_power_led(int color, int brightness);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_digital_output(int bits);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);

/*!
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
VF EXPORT_SYM int create_load_song(const unsigned char* song, const unsigned char length, const unsigned char num);

/*!
 * Plays a song that has been loaded. Use create_load_song first.
 * \param num The song slot to play from; valid values are 0, 1, 2, and 3
 * \return 1 on success, 0 on failure
 * \see create_load_song
 * \ingroup create
 */
VF EXPORT_SYM int create_play_song(const unsigned char num);

/*!
 * \ingroup create
 */
VF EXPORT_SYM int create_read_block(char *data, int count);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_write_byte(char byte);

/*!
 * \ingroup create
 */
VF EXPORT_SYM void create_clear_serial_buffer();

enum BaudRate
{
  Baud57600 = 0,
  Baud115200 = 1
};

/*!
 * \ingroup create
 */
VF EXPORT_SYM enum BaudRate get_create_baud_rate();

/*!
 * \ingroup create
 */
VF EXPORT_SYM void set_create_baud_rate(const enum BaudRate baudRate);

VFL

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_CREATE_H_ */

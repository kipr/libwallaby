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
 * \description Disconnects the controller from the iRobot Create.
 * \description ALWAYS disconnect from the iRobot Create before ending your program.
 * \see create_connect
 * \ingroup create
 */
VF EXPORT_SYM void create_disconnect();

/*!
 * \ingroup create
 * \see ceate_connect
 * \description Initiates a connection to the iRobot Create. 
 * \description Not recommended as create_connect() handles this and more.
 * \description Read iRobot Create manual before using this command.
 */ 
VF EXPORT_SYM void create_start();

/*!
 * \ingroup create
 * \see create_safe
 * \see create_full
 * \see get_create_mode
 * \description When the Create is in Passive mode, you can request and receive sensor data using any of the sensor commands, 
 * \description but you cannot change the current command parameters for the actuators (motors, speaker, lights, low side drivers, digital outputs) to something else. 
 * \description To change how one of the actuators operates, you must switch from Passive mode to Full mode or Safe mode.
 * \description While in Passive mode, you can read Roomba’s sensors, watch Roomba perform a cleaning cycle, and charge the battery. 
 * \description In Passive mode, Roomba will go into power saving mode to conserve battery power after five minutes of inactivity
 * \note Appears to crash controller, do not use or use serial communication.
 */
VF EXPORT_SYM void create_passive();

/*!
 * \ingroup create
 * \see create_full
 * \see create_passive
 * \see get_create_mode
 * \description Puts the iRobot Create in "Safe Mode"
 * \description Safe mode gives you full control of Roomba, with the exception of the following safety-related conditions.
 * \description safety-related conditions include: Cliff detection, wheel drop, and charging.
 * \description If your Create is refusing to move, try setting it to full mode.
 */
VF EXPORT_SYM void create_safe();

/*!
 * \ingroup create
 * \see create_safe
 * \see create_passive
 * \see get_create_mode
 * \description Puts the iRobot Create in "Full Mode"
 * \description "Full Mode" allows the programmer to completely control all functions of the Create (disables safety blocks).
 * \description With this mode enabled, the Create will not prevent use of motors in situations where it detects an edge/cliff/etc. 
 * \description Use this if you are having issues with the Create not moving after lifting it, falling, etc.
 * 
 */
VF EXPORT_SYM void create_full();

/*!
 * \ingroup create
 * \see create_demo
 * \see create_cover
 * \see create_cover_dock
 
 * \description Same as hitting the "Spot" button on the Roomba/Create.
 * \description Cleans a 3ft/1m circle around where SPOT was initiated.
 */
VF EXPORT_SYM void create_spot();

/*!
 * \ingroup create
 * \note Not Yet Implemented
 */
VF EXPORT_SYM void create_cover();

/*!
 * \ingroup create
 * \note Not Yet Implemented
 */
VF EXPORT_SYM void create_demo(int d);

/*!
 * \ingroup create
 * \note Not Yet Implemented
 */
VF EXPORT_SYM void create_cover_dock();

/*!
 * \ingroup create
 * \see create_safe
 * \see create_passive
 * \see create_full
 * \description Gets the current mode of the Create OI
 * \description Off: 0, Passive: 1, Safe: 2, Full: 3
 */
VF EXPORT_SYM int get_create_mode();

/*!
 * \ingroup create
 * \see get_create_rbump
 * \description Returns the status of the left bumper as a digital value (0 or 1).
 */
VF EXPORT_SYM int get_create_lbump();

/*!
 * \ingroup create
 * \see get_create_lbump
 * \description Returns the status of the right bumper as a digital value (0 or 1).
 */
VF EXPORT_SYM int get_create_rbump();

/*!
 * \ingroup create
 * \see get_create_rwdrop
 * \description Detects if the left wheel is dropped/lowered (the create is lifted)
 */
VF EXPORT_SYM int get_create_lwdrop();

/*!
 * \ingroup create
 * \see get_create_rwdrop
 * \see get_create_lwdrop
 * \note Not Implemented
 */
VF EXPORT_SYM int get_create_cwdrop();

/*!
 * \ingroup create
 * \see get_create_lwdrop
 * \description Detects if the right wheel is dropped/lowered (the create is lifted)
 */
VF EXPORT_SYM int get_create_rwdrop();

/*!
 * \ingroup create
 * \see get_create_vwall
 * \description Reports if the Create sees a physical wall.
 * \note The Create only detects walls on the right because Roombas only need it on the right side.
 */
VF EXPORT_SYM int get_create_wall();

/*!
 * \ingroup create
 * \see get_create_lfcliff
 * \see get_create_rfcliff
 * \see get_create_rcliff
 * \description reports if the left cliff/edge sensor is tripped. 
 */
VF EXPORT_SYM int get_create_lcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_rfcliff
 * \see get_create_rcliff
 * \description reports if the front-left cliff/edge sensor is tripped. 
 */
VF EXPORT_SYM int get_create_lfcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_rcliff
 * \see get_create_lfcliff
 * \description reports if the front-right cliff/edge sensor is tripped. 
 */
VF EXPORT_SYM int get_create_rfcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_lfcliff
 * \see get_create_rfcliff
 * \description reports if the right cliff/edge sensor is tripped. 
 */
VF EXPORT_SYM int get_create_rcliff();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_llightbump();

/*!
 * \ingroup create
 * \see get_create_llightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left-front light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_lflightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_llightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left-center light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_lclightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_llightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the right-center light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_rclightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_llightbump
 * \see get_create_rlightbump
 * \description returns the right-front light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_rflightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_llightbump
 * \description returns the right light bumper sensor as described in the iRobot Create manual.
 * \descritpion returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
VF EXPORT_SYM int get_create_rlightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump_amt
 * \see get_create_lclightbump_amt
 * \see get_create_rclightbump_amt
 * \see get_create_rflightbump_amt
 * \see get_create_rlightbump_amt
 * \description returns the left light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_llightbump_amt();

/*!
 * \ingroup create
 * \see get_create_lflightbump_amt
 * \see get_create_lclightbump_amt
 * \see get_create_rclightbump_amt
 * \see get_create_rflightbump_amt
 * \see get_create_llightbump_amt
 * \description returns the right light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_rlightbump_amt();

/*!
 * \ingroup create
 * \see get_create_llightbump_amt
 * \see get_create_lclightbump_amt
 * \see get_create_rclightbump_amt
 * \see get_create_rflightbump_amt
 * \see get_create_rlightbump_amt
 * \description returns the left-front light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_lflightbump_amt();

/*!
 * \ingroup create
 * \see get_create_lflightbump_amt
 * \see get_create_llightbump_amt
 * \see get_create_rclightbump_amt
 * \see get_create_rflightbump_amt
 * \see get_create_rlightbump_amt
 * \description returns the left-center light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_lclightbump_amt();

/*!
 * \ingroup create
 * \see get_create_lflightbump_amt
 * \see get_create_lclightbump_amt
 * \see get_create_llightbump_amt
 * \see get_create_rflightbump_amt
 * \see get_create_rlightbump_amt
 * \description returns the right-center light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_rclightbump_amt();

/*!
 * \ingroup create
 * \see get_create_lflightbump_amt
 * \see get_create_lclightbump_amt
 * \see get_create_rclightbump_amt
 * \see get_create_llightbump_amt
 * \see get_create_rlightbump_amt
 * \description returns the right-front light bumper sensor as described in the iRobot Create manual.
 * \description returns the amount the sensor is pressed, rather than a binary value.
 */
VF EXPORT_SYM int get_create_rflightbump_amt();

/*!
 * \ingroup create
 * \see get_create_wall
 * \description Reports if there is a virutal wall. 
 * \note The botball kit does not contain a "virtual wall" unit as of Fall 2019.
 */
VF EXPORT_SYM int get_create_vwall();

/*!
 * \ingroup create
 * \note Not Yet Implemented
 */
VF EXPORT_SYM int get_create_overcurrents();

/*!
 * \ingroup create
 * \description The Docks, Virutal Walls, etc. send out infrared signals that the Create can pick up.
 * \description For example, Right of dock ("Green Buoy"): 164, Left of Dock ("Red Buoy"): 168, "Force Field": 161
 * \description You can also potentially control the Create with IR signals for forward, backward, etc.
 * \description See iRobot Create Manual for more information ("Characters sent by iRobot devices")
 */
VF EXPORT_SYM int get_create_infrared();

/*!
 * \ingroup create
 * \description Returns if the "advance" button is pressed.
 * \note "Advance" is not a button on the newer Creates.
 */
VF EXPORT_SYM int get_create_advance_button();

/*!
 * \ingroup create
 * \description Returns if the "play" button is pressed.
 * \note "Play" is not a button on the newer Creates.
 */
VF EXPORT_SYM int get_create_play_button();

/*!
 * \ingroup create
 * \see set_create_normalized_angle
 * \description returns the normalized angle that the create is at in degrees (angle mod 360).
 * \description "Normalized" means that the angle is converted to be between 0 and 360 degrees first.
 */
VF EXPORT_SYM int get_create_normalized_angle();

/*!
 * \ingroup create
 * \see get_create_normalized_angle
 * \description Set the current angle that the create is at.
 * \description Sets what the create should use as a reference for its angle
 */
VF EXPORT_SYM void set_create_normalized_angle(int angle);

/*!
 * \ingroup create
 * \see set_create_total_angle
 * \description returns the angle the Create is currently turned to (does not "normalize").
 * \description This value is any value as it is not normalized to a range of [0,360)
 */
VF EXPORT_SYM int get_create_total_angle();

/*!
 * \ingroup create
 * \see get_create_total_angle
 * \description Set the current angle that the create is at for the total angle functions.
 * \description Sets what the create should use as a reference for its angle
 */
VF EXPORT_SYM void set_create_total_angle(int angle);

/*!
 * \ingroup create
 * \see set_create_distance
 * \description Gets the distance traveled based on encoder values in the wheels.
 * \description Units should be in millimeters (mm)
 */
VF EXPORT_SYM int get_create_distance();

/*!
 * \ingroup create
 * \description Sets the current distance the create thinks it has traveled.
 * \description Use this to set it's reference for where it is.
 */
VF EXPORT_SYM void set_create_distance(int dist);

/*!
 * \ingroup create
 * \see get_create_battery_voltage
 * \see get_create_battery_current
 * \description returns the current charging state of the battery.
 * \description Not Charging: 0, Reconditioning Charging: 1, Full Charging: 2, Trickle Charging: 3, Waiting: 4, Charging Connection Fault: 5
 */
VF EXPORT_SYM int get_create_battery_charging_state();

/*!
 * \ingroup create
 * \description returns battery voltage in millivolts (mV)
 */
VF EXPORT_SYM int get_create_battery_voltage();

/*!
 * \ingroup create
 * \description returns current in milliAmps (mA)
 */
VF EXPORT_SYM int get_create_battery_current();

/*!
 * \ingroup create
 * \description returns the temperature of the battery in degrees Celcius
 */
VF EXPORT_SYM int get_create_battery_temp();

/*!
 * \ingroup create
 * \description returns the current charge on the battery in milliAmp-Hours (mAH)
 */
VF EXPORT_SYM int get_create_battery_charge();

/*!
 * \ingroup create
 * \description Returns the capacity of the battery in milliAmp-Hours (mAH)
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

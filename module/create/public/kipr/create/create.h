/*!
 * \file create.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup create iRobot (R) Create (TM)
 */
/**
 * \page create
 * The functions here allow for communication with the Create (roomba).
 * They only work when there is a Create cable plugged into the
 * wombat and into the Create.
 * \section setup Setup
 * Make sure to plug the usb side of the Create cable into the
 * wombat and the other side into the Create.
 * \section example_create_movement Movement Example
 * Once you've done that, you can now use the roomba. The
 * following example moves the roomba forward, swerves left, 
 * swerves right, and finally drives backwards.
 * ```
 * #include <kipr/wombat.h>
 * #include <stdio.h>
 * 
 * int main(){
 *      // connect to the create in order to control it
 *      create_connect_once();
 * 
 *      // move forward for 1 second
 *      create_drive_direct(200, 200);
 *      msleep(1000);
 * 
 *      // swerve left
 *      create_drive_direct(100, 200);
 *      msleep(300);
 *      // swerve right
 *      create_drive_direct(200, 100);
 *      msleep(300);
 *  
 *      // go backwards for 1 second
 *      create_drive_direct(-100, -100);
 *      msleep(1000);
 * 
 *      // stop moving
 *      create_drive_direct(0, 0);
 * 
 *      // disconnect before ending the program
 *      create_disconnect();
 *      return 0;
 * }
 * ```
 * \ingroup create
 */

#ifndef _KIPR_CREATE_CREATE_H_
#define _KIPR_CREATE_CREATE_H_

#include "kipr/export/export.h"

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
EXPORT_SYM int create_connect();

/*!
 * Attempts to establish a connection to the create.
 * \return 1 if connection succeeded, 0 if connection failed
 * \see create_disconnect
 * \ingroup create
 */
EXPORT_SYM int create_connect_once();

/*!
 * \description Disconnects the controller from the iRobot Create.
 * \description ALWAYS disconnect from the iRobot Create before ending your program.
 * \see create_connect
 * \ingroup create
 */
EXPORT_SYM void create_disconnect();

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
EXPORT_SYM void create_passive();

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
EXPORT_SYM void create_safe();

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
EXPORT_SYM void create_full();

/*!
 * \ingroup create
 * \see create_safe
 * \see create_passive
 * \see create_full
 * \description Gets the current mode of the Create OI
 * \description Off: 0, Passive: 1, Safe: 2, Full: 3
 */
EXPORT_SYM int get_create_mode();

/*!
 * \ingroup create
 * \see get_create_rbump
 * \description Returns the status of the left bumper as a digital value (0 or 1).
 */
EXPORT_SYM int get_create_lbump();

/*!
 * \ingroup create
 * \see get_create_lbump
 * \description Returns the status of the right bumper as a digital value (0 or 1).
 */
EXPORT_SYM int get_create_rbump();

/*!
 * \ingroup create
 * \see get_create_rwdrop
 * \description Detects if the left wheel is dropped/lowered (the create is lifted)
 */
EXPORT_SYM int get_create_lwdrop();

/*!
 * \ingroup create
 * \see get_create_rwdrop
 * \see get_create_lwdrop
 * \note Not Implemented
 */
EXPORT_SYM int get_create_cwdrop();

/*!
 * \ingroup create
 * \see get_create_lwdrop
 * \description Detects if the right wheel is dropped/lowered (the create is lifted)
 */
EXPORT_SYM int get_create_rwdrop();

/*!
 * \ingroup create
 * \see get_create_vwall
 * \description Reports if the Create sees a physical wall.
 * \note The Create only detects walls on the right because Roombas only need it on the right side.
 */
EXPORT_SYM int get_create_wall();

/*!
 * \ingroup create
 * \see get_create_lfcliff
 * \see get_create_rfcliff
 * \see get_create_rcliff
 * \description reports if the left cliff/edge sensor is tripped. 
 */
EXPORT_SYM int get_create_lcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_rfcliff
 * \see get_create_rcliff
 * \description reports if the front-left cliff/edge sensor is tripped. 
 */
EXPORT_SYM int get_create_lfcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_rcliff
 * \see get_create_lfcliff
 * \description reports if the front-right cliff/edge sensor is tripped. 
 */
EXPORT_SYM int get_create_rfcliff();

/*!
 * \ingroup create
 * \see get_create_lcliff
 * \see get_create_lfcliff
 * \see get_create_rfcliff
 * \description reports if the right cliff/edge sensor is tripped. 
 */
EXPORT_SYM int get_create_rcliff();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_llightbump();

/*!
 * \ingroup create
 * \see get_create_llightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left-front light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_lflightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_llightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the left-center light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_lclightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_llightbump
 * \see get_create_rflightbump
 * \see get_create_rlightbump
 * \description returns the right-center light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_rclightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_llightbump
 * \see get_create_rlightbump
 * \description returns the right-front light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_rflightbump();

/*!
 * \ingroup create
 * \see get_create_lflightbump
 * \see get_create_lclightbump
 * \see get_create_rclightbump
 * \see get_create_rflightbump
 * \see get_create_llightbump
 * \description returns the right light bumper sensor as described in the iRobot Create manual.
 * \description returns a binary/digital value rather than the raw sensor data (on or off/1 or 0).
 */
EXPORT_SYM int get_create_rlightbump();

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
EXPORT_SYM int get_create_llightbump_amt();

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
EXPORT_SYM int get_create_rlightbump_amt();

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
EXPORT_SYM int get_create_lflightbump_amt();

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
EXPORT_SYM int get_create_lclightbump_amt();

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
EXPORT_SYM int get_create_rclightbump_amt();

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
EXPORT_SYM int get_create_rflightbump_amt();

/*!
 * \ingroup create
 * \see get_create_wall
 * \description Reports if there is a virutal wall. 
 * \note The botball kit does not contain a "virtual wall" unit as of Fall 2019.
 */
EXPORT_SYM int get_create_vwall();

/*!
 * \ingroup create
 * \description The Docks, Virutal Walls, etc. send out infrared signals that the Create can pick up.
 * \description For example, Right of dock ("Green Buoy"): 164, Left of Dock ("Red Buoy"): 168, "Force Field": 161
 * \description You can also potentially control the Create with IR signals for forward, backward, etc.
 * \description See iRobot Create Manual for more information ("Characters sent by iRobot devices")
 */
EXPORT_SYM int get_create_infrared();

/*!
 * \ingroup create
 * \description Returns if the "advance" button is pressed.
 * \note "Advance" is not a button on the newer Creates.
 */
EXPORT_SYM int get_create_advance_button();

/*!
 * \ingroup create
 * \description Returns if the "play" button is pressed.
 * \note "Play" is not a button on the newer Creates.
 */
EXPORT_SYM int get_create_play_button();

/*!
 * \ingroup create
 * \see set_create_normalized_angle
 * \description returns the normalized angle that the create is at in degrees (angle mod 360).
 * \description "Normalized" means that the angle is converted to be between 0 and 360 degrees first.
 */
EXPORT_SYM int get_create_normalized_angle();

/*!
 * \ingroup create
 * \see get_create_normalized_angle
 * \description Set the current angle that the create is at.
 * \description Sets what the create should use as a reference for its angle
 */
EXPORT_SYM void set_create_normalized_angle(int angle);

/*!
 * \ingroup create
 * \see set_create_total_angle
 * \description returns the angle the Create is currently turned to (does not "normalize").
 * \description This value is any value as it is not normalized to a range of [0,360)
 */
EXPORT_SYM int get_create_total_angle();

/*!
 * \ingroup create
 * \see get_create_total_angle
 * \description Set the current angle that the create is at for the total angle functions.
 * \description Sets what the create should use as a reference for its angle
 */
EXPORT_SYM void set_create_total_angle(int angle);

/*!
 * \ingroup create
 * \see set_create_distance
 * \description Gets the distance traveled based on encoder values in the wheels.
 * \description Units should be in millimeters (mm)
 */
EXPORT_SYM int get_create_distance();

/*! 
 * \ingroup create
 * \description Sets the current distance the create thinks it has traveled.
 * \description Use this to set it's reference for where it is.
 */
EXPORT_SYM void set_create_distance(int dist);

/*!
 * \ingroup create
 * \see get_create_battery_voltage
 * \see get_create_battery_current
 * \description returns the current charging state of the battery.
 * \description Not Charging: 0, Reconditioning Charging: 1, Full Charging: 2, Trickle Charging: 3, Waiting: 4, Charging Connection Fault: 5
 */
EXPORT_SYM int get_create_battery_charging_state();

/*!
 * \ingroup create
 * \description returns the temperature of the battery in degrees Celcius
 */
EXPORT_SYM int get_create_battery_temp();

/*!
 * \ingroup create
 * \description returns the current charge on the battery in milliAmp-Hours (mAH)
 */
EXPORT_SYM int get_create_battery_charge();

/*!
 * \ingroup create
 * \description Returns the capacity of the battery in milliAmp-Hours (mAH)
 */
EXPORT_SYM int get_create_battery_capacity();

/*!
 * Returns the wall signal sensed by the create.
 * \see get_create_wall
 * \ingroup create
 */
EXPORT_SYM int get_create_wall_amt();

/*!
 * Returns the value measured by the left-most
 * cliff sensor.
 * \note For line following purposes, this can be treated like
 * a tophat sensor value. Lower values often indicate black, higher
 * values often indicate white.
 * \see get_create_lcliff
 * \ingroup create
 */
EXPORT_SYM int get_create_lcliff_amt();

/*!
 * Returns the value measured by the left-front
 * cliff sensor.
 * \note For line following purposes, this can be treated like
 * a tophat sensor value. Lower values often indicate black, higher
 * values often indicate white.
 * \see get_create_lfcliff
 * \ingroup create
 */
EXPORT_SYM int get_create_lfcliff_amt();

/*!
 * Returns the value measured by the right-front
 * cliff sensor.
 * \note For line following purposes, this can be treated like
 * a tophat sensor value. Lower values often indicate black, higher
 * values often indicate white.
 * \see get_create_rfcliff
 * \ingroup create
 */
EXPORT_SYM int get_create_rfcliff_amt();

/*!
 * Returns the value measured by the right-most
 * cliff sensor.
 * \note For line following purposes, this can be treated like
 * a tophat sensor value. Lower values often indicate black, higher
 * values often indicate white.
 * \ingroup create
 */
EXPORT_SYM int get_create_rcliff_amt();

/*!
 * \return The song slot of the current song
 * \ingroup create
 */
EXPORT_SYM int get_create_song_number();

/*!
 * \return 1 if it the create is playing a song,
 * 0 if the create isn't playing a song
 * \ingroup create
 */
EXPORT_SYM int get_create_song_playing();

/*!
 * Sets the create connection mode to off. No sensor data
 * will be available, and the create will not respond to
 * movement commands.
 * \ingroup create
 */
EXPORT_SYM void create_stop();

/*!
 * Drive at the requested speed in an arc with the provided
 * radius.
 * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
 * \param radius The radius (in mm) of the arc to drive through. Range is
 * -2000 to 2000.
 * \ingroup create
 */
EXPORT_SYM void create_drive(int speed, int radius);

/*!
 * Drive straight at the requested speed.
 * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
 * \ingroup create
 */
EXPORT_SYM void create_drive_straight(int speed);

/*!
 * Spin clockwise at the requested speed. Spins in place.
 * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
 * \note negative speeds will result in spinning counter-clockwise
 * \ingroup create
 */
EXPORT_SYM void create_spin_CW(int speed);

/*!
 * Spin counter-clockwise at the requested speed. Spins in place.
 * \param speed The speed (in mm/s) to drive at. Range is -500 to 500.
 * \note negative speeds will result in spinning clockwise
 * \ingroup create
 */
EXPORT_SYM void create_spin_CCW(int speed);

/*!
 * Drive the create at the provided left wheel speed and right wheel speed.
 * \param l_speed The speed (in mm/s) to drive the left wheel at. Range is -500 to 500
 * \param r_speed The speed (in mm/s) to drive the right wheel at. Range is -500 to 500
 * \ingroup create
 */
EXPORT_SYM void create_drive_direct(int l_speed, int r_speed);

/*!
 * Turns the create the requested number of degrees at the requested speed.
 * \param speed The speed (in mm/s) to turn at. Range is -500 to 500
 * \param angle The angle (in degrees) to turn.
 * \note This is a blocking function, so you DO NOT need an msleep after it.
 * It already sleeps for the time that it needs to execute the turn and
 * doesn't need extra msleeps.
 * \ingroup create
 */
EXPORT_SYM void create_spin_block(int speed, int angle);

/*!
 * Gets the displacement of the left wheel and right wheel and
 * puts that data into the `long`s provided
 * \param[out] lenc - where the left wheel's displacement is stored
 * \param[out] renc - where the right wheel's displacement is stored 
 * \note Not yet implemented
 * \ingroup create
 */
EXPORT_SYM int _create_get_raw_encoders(long *lenc, long *renc);

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
EXPORT_SYM int create_load_song(const unsigned char* song, const unsigned char length, const unsigned char num);

/*!
 * Plays a song that has been loaded. Use create_load_song first.
 * \param num The song slot to play from; valid values are 0, 1, 2, and 3
 * \return 1 on success, 0 on failure
 * \see create_load_song
 * \ingroup create
 */
EXPORT_SYM int create_play_song(const unsigned char num);

/*!
 * Reads sensor data from the create.
 * \param data - This is where the read sensor data is stored
 * \param count - How many bytes to read from the create.
 * \returns 1 on success, 0 on failure.
 * \ingroup create
 */
EXPORT_SYM int create_read_block(char *data, int count);

/*!
 * Write a byte to the create. This is used to send commands directly
 * to the create without using the functions provided in this library.
 * \note If you want to directly send bytes to the create, consider checking out
 * the actual create oi specifications: https://cdn-shop.adafruit.com/datasheets/create_2_Open_Interface_Spec.pdf
 * \ingroup create
 */
EXPORT_SYM void create_write_byte(char byte);

/*!
 * Flushes commands to the create.
 * \note This library already automatically flushes commands to the create
 * \ingroup create
 */
EXPORT_SYM void create_clear_serial_buffer();

/**
 * \brief The baudrate to use with the create
 * \details The baudrate can be thought of as the refresh
 * rate, or how many times per second the wombat/wallaby
 * checks the create's sensors and sends commands (if any)
 * to the create
 */
enum BaudRate
{
  Baud57600 = 0,  //!< Baudrate of 57600 times per second
  Baud115200 = 1  //!< Baudrate of 115200 times per second
};


/*!
 * Returns the baud rate of the create.
 * \note The baud rate is how many times a second the create updates its sensors and
 * receives commands.
 * \ingroup create
 */
EXPORT_SYM enum BaudRate get_create_baud_rate();

/*!
 * Sets the create's baud rate to the provided baud rate.
 * \ingroup create
 */
EXPORT_SYM void set_create_baud_rate(const enum BaudRate baudRate);

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_CREATE_H_ */

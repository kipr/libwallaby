/*
 * motor_p.hpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#ifndef SRC_MOTOR_P_HPP_
#define SRC_MOTOR_P_HPP_

namespace Private
{

// TODO:  merge
namespace Motor
{
  enum MotorControlMode
  {
    Inactive = 0,
    Speed,
    Position,
    SpeedPosition
  };

  enum MotorDirection
  {
    PassiveStop = 0,
    Forward,
    Reverse,
    ActiveStop
  };
}

unsigned char get_motor_mode(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_mode(unsigned int port, unsigned char mode);

bool clear_motor_bemf(unsigned int port);

int get_motor_bemf(unsigned int port, unsigned char * alt_read_buffer);

int get_motor_bemf_vel(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_direction(unsigned int port, unsigned char dir);

unsigned char get_motor_direction(unsigned int port, unsigned char * alt_read_buffer);

bool stop_motor(int port);

unsigned int get_motor_pwm(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_pwm(unsigned int port, unsigned char speed);

int get_motor_goal_velocity(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_goal_velocity(unsigned int port, int goal_velocity);

int get_motor_goal_position(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_goal_position(unsigned int port, int goal_position);

bool set_motor_pid_gains(int port, short p, short i, short d, short pd, short id, short dd);

bool get_motor_pid_gains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd);

bool get_motor_stop(unsigned int port, unsigned char * alt_read_buffer);

bool set_motor_stop(unsigned int port, bool stop);

bool get_motor_done(unsigned int port, unsigned char * alt_read_buffer);

bool get_motor_pid_active(unsigned int port, unsigned char * alt_read_buffer);

}

#endif /* SRC_MOTOR_P_HPP_ */

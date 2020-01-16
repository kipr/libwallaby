/*
 * motor_p.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "motor_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"

#include <iostream>

namespace Private
{
static const unsigned int NUM_MOTORS = 4; // TODO: get this from somewhere

typedef struct pid_coeffs_t
{
  int p,i,d,pd,id,dd;
} pid_coeffs;

static pid_coeffs pid_coeff_array[NUM_MOTORS] = {
    {2000,200,1,1000,1000,1000},
    {2000,200,1,1000,1000,1000},
    {2000,200,1,1000,1000,1000},
    {2000,200,1,1000,1000,1000}};

static int goal_pos_array[NUM_MOTORS] = {0,0,0,0};
static int goal_vel_array[NUM_MOTORS] = {0,0,0,0};

static int update_Hz = 250;

int per_tick_small_to_large(int val)
{
  return val * update_Hz;
}

int per_tick_large_to_small(int val)
{
  return val / update_Hz;
}

// TODO: this is a hack but the hardware timers are making it hard to easily swap motors 2,3 today
unsigned int fix_port(unsigned int port)
{
  if (port == 2) return 3;
  if (port == 3) return 2;
  return port;
}

unsigned char get_motor_mode(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port < 0 || port > 3) return Motor::Inactive; // TODO: better fail code

  const unsigned short offset = 2*fix_port(port);

  unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES, alt_read_buffer);
  unsigned char mode = (modes & (0x3 << offset)) >> offset;

  return (Motor::MotorControlMode)mode;

  return mode; // FIXME
}


bool set_motor_mode(unsigned int port, unsigned char mode)
{
  if (port >= NUM_MOTORS) return false;

  if (mode != 0) std::lock_guard<std::mutex> lock(shutdown_mutex);

  unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES);

  const unsigned short offset = 2*fix_port(port);

  // clear old drive mode
  modes &= ~(0x3 << offset);

  // set new drive mode
  modes |=  (((int)mode) << offset);

  Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_MODES, modes);

  return true;
}

bool clear_motor_bemf(unsigned int port)
{
  if (port >= NUM_MOTORS) return false;
  Private::Wallaby::instance()->writeRegister32b(REG_RW_MOT_0_B3 + 4 * fix_port(port), 0);
  return true;
}

int get_motor_bemf(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return 0;
  int val = Private::Wallaby::instance()->readRegister32b(REG_RW_MOT_0_B3 + 4 * fix_port(port), alt_read_buffer);
  return per_tick_large_to_small(val); // TODO: cleaner place for scaling
}

int get_motor_bemf_vel(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return 0;
  int val = Private::Wallaby::instance()->readRegister32b(REG_RW_MOT_0_SP_H + 4 * fix_port(port), alt_read_buffer);
  return val;
}

bool set_motor_direction(unsigned int port, unsigned char dir)
{
  if (port >= NUM_MOTORS) return false;

  Private::set_motor_mode(port, Motor::Inactive);

  if (dir != Motor::Inactive) std::lock_guard<std::mutex> lock(shutdown_mutex);

  unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS);

  unsigned short offset = 2 * fix_port(port);

  dirs &= ~(0x3 << offset);

  dirs |= (dir << offset);

  Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_DIRS, dirs);
  return true;
}

unsigned char get_motor_direction(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return Motor::PassiveStop;
  // TODO: error signal outside of range

  unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS, alt_read_buffer);

  unsigned short offset = 2 * fix_port(port);

  return (Motor::MotorDirection)((dirs & (0x3 << offset)) >> offset);
}

bool stop_motor(int port)
{
  if (port >= NUM_MOTORS) return false;
  Private::set_motor_mode(port, Motor::Inactive);
  Private::set_motor_direction(port, Motor::PassiveStop);
  return true;
}


unsigned int get_motor_pwm(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return 0;
  // TODO: error signal outside of range
  return Private::Wallaby::instance()->readRegister16b(REG_RW_MOT_0_PWM_H + 2 * fix_port(port), alt_read_buffer);
}

bool set_motor_pwm(unsigned int port, unsigned char speed)
{

  if (port >= NUM_MOTORS) return false;
  std::lock_guard<std::mutex> lock(shutdown_mutex);
  // TODO: error signal outside of range
  Private::set_motor_mode(port, Motor::Inactive);
  const unsigned short speedMax = 400;
  unsigned short adjustedSpeed = speed * 4;
  if (adjustedSpeed > speedMax) adjustedSpeed = speedMax; // TODO: check scaling (1/4 percent increments)
  Private::Wallaby::instance()->writeRegister16b(REG_RW_MOT_0_PWM_H + 2 * fix_port(port), adjustedSpeed);

  return true;
}


int get_motor_goal_velocity(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return 0; // TODO

  return goal_vel_array[port];
}

bool set_motor_goal_velocity(unsigned int port, int goal_velocity)
{
  if (port >= NUM_MOTORS) return false;

  // TODO:check that byte order doesn't get messed up
  // TODO: may need to put some logic in for not writing goals if they equal the current goal
  //  ... maybe add on the co-proc?
  unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * fix_port(port); // TODO: 32 bit?
  std::lock_guard<std::mutex> lock(shutdown_mutex);
  Private::Wallaby::instance()->writeRegister16b(goal_addy, static_cast<signed short>(goal_velocity));

  goal_vel_array[port] = goal_velocity;

  return true;
}

int get_motor_goal_position(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return 0; // TODO

  return goal_pos_array[port];
}

bool set_motor_goal_position(unsigned int port, int goal_position)
{
  if (port >= NUM_MOTORS) return false;

  // TODO
  // TODO: logic to not set goals if they match the current value (in co-proc firmware maybe?)
  unsigned int goal_addy = REG_W_MOT_0_GOAL_B3 + 4 * fix_port(port);
  Private::Wallaby::instance()->writeRegister32b(goal_addy, per_tick_small_to_large(goal_position));

  goal_pos_array[port] = goal_position;

  return true;
}



bool set_motor_pid_gains(int port, short p, short i, short d, short pd, short id, short dd)
{
  if (port >= NUM_MOTORS) return false;

  unsigned int addy_offset = 12 * fix_port(port);  // 6 registers at 2 bytes each... per port
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_P_H + addy_offset, p);
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_I_H + addy_offset, i);
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_D_H + addy_offset, d);
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_PD_H + addy_offset, pd);
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_ID_H + addy_offset, id);
  Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_DD_H + addy_offset, dd);

  pid_coeff_array[port].p = p;
  pid_coeff_array[port].i = i;
  pid_coeff_array[port].d = d;
  pid_coeff_array[port].pd = pd;
  pid_coeff_array[port].id = id;
  pid_coeff_array[port].dd = dd;

  return true;
}

bool get_motor_pid_gains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd)
{
  if (port >= NUM_MOTORS) return false;

  p = pid_coeff_array[port].p;
  i = pid_coeff_array[port].i;
  d = pid_coeff_array[port].d;
  pd = pid_coeff_array[port].pd;
  id= pid_coeff_array[port].id;
  dd = pid_coeff_array[port].dd;

  return true;
}

bool get_motor_stop(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return false;
  // TODO: this needs testing
  bool stopped = Private::get_motor_mode(port, alt_read_buffer) == Motor::Inactive
      && Private::get_motor_direction(port, alt_read_buffer) == Motor::PassiveStop;
  return stopped;
}

bool set_motor_stop(unsigned int port, bool stop)
{
  if (port >= NUM_MOTORS) return false;
  Private::set_motor_mode(port, Motor::Inactive);
  Private::set_motor_direction(port, Motor::PassiveStop);
  return true;
}


bool get_motor_done(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return false;
  unsigned char motors_done = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DONE, alt_read_buffer);
  //std::cout << "motors done = " << (unsigned int)motors_done << " vs " << (1 < fix_port(port))  << "fixed port is " << std::endl;

  return (motors_done & (1 << fix_port(port)));
}

bool get_motor_pid_active(unsigned int port, unsigned char * alt_read_buffer)
{
  if (port >= NUM_MOTORS) return false;
  return !(Private::get_motor_done(port, alt_read_buffer));
}
}

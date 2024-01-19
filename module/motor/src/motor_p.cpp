/*
 * motor_p.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Joshua Southerland
 */

#include "motor_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/core/cleanup.hpp"

#include <iostream>

using namespace kipr;
using namespace kipr::motor;

using kipr::core::Platform;
using kipr::core::cleanup_mutex;

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
  #ifndef EMSCRIPTEN
  if (port == 2) return 3;
  if (port == 3) return 2;
  #endif
  
  return port;
}

unsigned char kipr::motor::get_motor_mode(unsigned int port)
{
  if (port < 0 || port > 3) return static_cast<unsigned char>(ControlMode::Inactive); // TODO: better fail code

  const unsigned short offset = 2*fix_port(port);

  unsigned char modes = Platform::instance()->readRegister8b(REG_RW_MOT_MODES);
  unsigned char mode = (modes & (0x3 << offset)) >> offset;

  return mode;
}


bool kipr::motor::set_motor_mode(unsigned int port, unsigned char mode)
{
  if (port >= NUM_MOTORS) return false;

  if (mode != 0) std::lock_guard<std::mutex> lock(cleanup_mutex);

  unsigned char modes = Platform::instance()->readRegister8b(REG_RW_MOT_MODES);

  const unsigned short offset = 2*fix_port(port);

  // clear old drive mode
  modes &= ~(0x3 << offset);

  // set new drive mode
  modes |=  (((int)mode) << offset);

  Platform::instance()->writeRegister8b(REG_RW_MOT_MODES, modes);

  return true;
}

bool kipr::motor::clear_motor_bemf(unsigned int port)
{
  if (port >= NUM_MOTORS) return false;
  Platform::instance()->writeRegister32b(REG_RW_MOT_0_B3 + 4 * fix_port(port), 0);
  return true;
}

int kipr::motor::get_motor_bemf(unsigned int port)
{
  if (port >= NUM_MOTORS) return 0;
  int val = Platform::instance()->readRegister32b(REG_RW_MOT_0_B3 + 4 * fix_port(port));
  return per_tick_large_to_small(val); // TODO: cleaner place for scaling
}

int kipr::motor::get_motor_bemf_vel(unsigned int port)
{
  if (port >= NUM_MOTORS) return 0;
  int val = Platform::instance()->readRegister32b(REG_RW_MOT_0_SP_H + 4 * fix_port(port));
  return val;
}

bool kipr::motor::set_motor_direction(unsigned int port, unsigned char dir)
{
  if (port >= NUM_MOTORS) return false;

  set_motor_mode(port, static_cast<unsigned char>(ControlMode::Inactive));

  unsigned char dirs = Platform::instance()->readRegister8b(REG_RW_MOT_DIRS);

  unsigned short offset = 2 * fix_port(port);

  dirs &= ~(0x3 << offset);

  dirs |= (dir << offset);

  Platform::instance()->writeRegister8b(REG_RW_MOT_DIRS, dirs);
  return true;
}

unsigned char kipr::motor::get_motor_direction(unsigned int port)
{
  if (port >= NUM_MOTORS) return static_cast<unsigned char>(Direction::PassiveStop);
  // TODO: error signal outside of range

  unsigned char dirs = Platform::instance()->readRegister8b(REG_RW_MOT_DIRS);

  unsigned short offset = 2 * fix_port(port);

  return ((dirs & (0x3 << offset)) >> offset);
}

bool kipr::motor::stop_motor(int port)
{
  if (port >= NUM_MOTORS) return false;
  set_motor_mode(port, static_cast<unsigned char>(ControlMode::Inactive));
  set_motor_direction(port, static_cast<unsigned char>(Direction::PassiveStop));
  return true;
}

unsigned int kipr::motor::get_motor_pwm(unsigned int port)
{
  if (port >= NUM_MOTORS) return 0;
  // TODO: error signal outside of range
  return Platform::instance()->readRegister16b(REG_RW_MOT_0_PWM_H + 2 * fix_port(port));
}

bool kipr::motor::set_motor_pwm(unsigned int port, unsigned char speed)
{

  if (port >= NUM_MOTORS) return false;
  std::lock_guard<std::mutex> lock(cleanup_mutex);
  // TODO: error signal outside of range
  set_motor_mode(port, static_cast<unsigned char>(ControlMode::Inactive));
  const unsigned short speedMax = 400;
  unsigned short adjustedSpeed = speed * 4;
  if (adjustedSpeed > speedMax) adjustedSpeed = speedMax; // TODO: check scaling (1/4 percent increments)
  Platform::instance()->writeRegister16b(REG_RW_MOT_0_PWM_H + 2 * fix_port(port), adjustedSpeed);

  return true;
}


int get_motor_goal_velocity(unsigned int port)
{
  if (port >= NUM_MOTORS) return 0; // TODO

  return goal_vel_array[port];
}

bool kipr::motor::set_motor_goal_velocity(unsigned int port, int goal_velocity)
{
  if(goal_velocity > 1500)
  {
	  goal_velocity = 1500;
  }
  
  if(goal_velocity < -1500)
  {
	  goal_velocity = -1500;
  }

  if (port >= NUM_MOTORS) return false;

  // TODO:check that byte order doesn't get messed up
  // TODO: may need to put some logic in for not writing goals if they equal the current goal
  //  ... maybe add on the co-proc?
  unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * fix_port(port); // TODO: 32 bit?
  std::lock_guard<std::mutex> lock(cleanup_mutex);
  Platform::instance()->writeRegister16b(goal_addy, static_cast<signed short>(goal_velocity));

  goal_vel_array[port] = goal_velocity;

  return true;
}

int kipr::motor::get_motor_goal_position(unsigned int port)
{
  if (port >= NUM_MOTORS) return 0; // TODO

  return goal_pos_array[port];
}

bool kipr::motor::set_motor_goal_position(unsigned int port, int goal_position)
{
  if (port >= NUM_MOTORS) return false;

  // TODO
  // TODO: logic to not set goals if they match the current value (in co-proc firmware maybe?)
  unsigned int goal_addy = REG_W_MOT_0_GOAL_B3 + 4 * fix_port(port);
  Platform::instance()->writeRegister32b(goal_addy, per_tick_small_to_large(goal_position));

  goal_pos_array[port] = goal_position;

  return true;
}



bool kipr::motor::set_motor_pid_gains(int port, short p, short i, short d, short pd, short id, short dd)
{
  if (port >= NUM_MOTORS) return false;

  unsigned int addy_offset = 12 * fix_port(port);  // 6 registers at 2 bytes each... per port
  Platform::instance()->writeRegister16b(REG_W_PID_0_P_H + addy_offset, p);
  Platform::instance()->writeRegister16b(REG_W_PID_0_I_H + addy_offset, i);
  Platform::instance()->writeRegister16b(REG_W_PID_0_D_H + addy_offset, d);
  Platform::instance()->writeRegister16b(REG_W_PID_0_PD_H + addy_offset, pd);
  Platform::instance()->writeRegister16b(REG_W_PID_0_ID_H + addy_offset, id);
  Platform::instance()->writeRegister16b(REG_W_PID_0_DD_H + addy_offset, dd);

  pid_coeff_array[port].p = p;
  pid_coeff_array[port].i = i;
  pid_coeff_array[port].d = d;
  pid_coeff_array[port].pd = pd;
  pid_coeff_array[port].id = id;
  pid_coeff_array[port].dd = dd;

  return true;
}

bool kipr::motor::get_motor_pid_gains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd)
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

bool kipr::motor::get_motor_stop(unsigned int port)
{
  if (port >= NUM_MOTORS) return false;
  // TODO: this needs testing
  bool stopped = get_motor_mode(port) == static_cast<unsigned char>(ControlMode::Inactive)
      && get_motor_direction(port) == static_cast<unsigned char>(Direction::PassiveStop);
  return stopped;
}

bool kipr::motor::set_motor_stop(unsigned int port, bool stop)
{
  if (port >= NUM_MOTORS) return false;
  set_motor_mode(port, static_cast<unsigned char>(ControlMode::Inactive));
  set_motor_direction(port, static_cast<unsigned char>(Direction::PassiveStop));
  return true;
}


bool kipr::motor::get_motor_done(unsigned int port)
{
  if (port >= NUM_MOTORS) return false;
  unsigned char motors_done = Platform::instance()->readRegister8b(REG_RW_MOT_DONE);

  return (motors_done & (1 << fix_port(port)));
}

bool kipr::motor::get_motor_pid_active(unsigned int port)
{
  if (port >= NUM_MOTORS) return false;
  return !(get_motor_done(port));
}

/*
 * motors_p.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#include "motors_p.hpp"
#include "wallaby_p.hpp"
#include "wallaby_regs_p.hpp"


using namespace Private;


Motor::~Motor()
{

}

void Motor::setPidGains(int port, short p, short i, short d, short pd, short id, short dd)
{
	if (port < 0 || port > 3) return;

	unsigned int addy_offset = 12 * port;  // 6 registers at 2 bytes each... per port
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_P_H + addy_offset, p);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_I_H + addy_offset, i);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_D_H + addy_offset, d);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_PD_H + addy_offset, pd);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_ID_H + addy_offset, id);
	Private::Wallaby::instance()->writeRegister16b(REG_W_PID_0_DD_H + addy_offset, dd);
}

void Motor::clearBemf(int port)
{
	if (port < 0 || port > 3) return;

	//TODO: FIXME what is this function's purpose?
}

void Motor::setControlMode(int port, Motor::ControlMode mode)
{
	if (port < 0 || port > 3) return;
	unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES);

	const unsigned short offset = 2*port;

	// clear old drive mode
	modes &= ~(0x3 << offset);

	// set new drive mode
	modes |=  (((int)mode) << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_MODES, modes);
}

Motor::ControlMode Motor::controlMode(int port) const
{
	if (port < 0 || port > 3) return Motor::ControlMode::Inactive; // TODO: better fail code
	unsigned char modes = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_MODES);

	const unsigned short offset = 2*port;

	//
	unsigned char mode = (modes & (0x3 << offset)) >> offset;

	return (Private::Motor::ControlMode)mode;
}

bool Motor::isPidActive(int port) const
{
	unsigned char motor_done = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DONE);
	return !(motor_done & (1 < port));
}

void Motor::setPidVelocity(int port, int ticks)
{
	// TODO:check that byte order doesn't get messed up
	// TODO: may need to put some logic in for not writing goals if they equal the current goal
	//  ... maybe add on the co-proc?
	unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * port; // TODO: 32 bit?
	Private::Wallaby::instance()->writeRegister16b(goal_addy, static_cast<signed short>(ticks));

}

int Motor::pidVelocity(int port) const
{
	// TODO: 32 bit?
	unsigned int goal_addy = REG_RW_MOT_0_SP_H + 2 * port;
	int val = Private::Wallaby::instance()->readRegister16b(goal_addy);
	return val;
}

void Motor::setPidGoalPos(int port, int pos)
{
	// TODO: do we really need 64 bit positions?
	// TODO: logic to not set goals if they match the current value (in co-proc firmware maybe?)
	unsigned int goal_addy = REG_W_MOT_0_GOAL_B3 + 4 * port;
	Private::Wallaby::instance()->writeRegister32b(goal_addy, pos);
}


int Motor::pidGoalPos(int port) const
{
	// FIXME: last commanded goal pos since we can't read it
	return 0;
}

void Motor::pidGains(int port, short & p, short & i, short & d, short & pd, short & id, short & dd) const
{
	if (port < 0 || port > 3) return;
	//unsigned short offset = port * 2 * 3;
	//TODO we can't read them from co-proc
}

void Motor::setPwm(int port, unsigned short speed)
{
	if (port < 0 || port > 3) return;
	// TODO: error signal outside of range
	setControlMode(port, Private::Motor::Inactive);
	const unsigned short speedMax = 400;
	unsigned short adjustedSpeed = speed * 4;
	if (adjustedSpeed > speedMax) adjustedSpeed = speedMax; // TODO: check scaling (1/4 percent increments)
	Private::Wallaby::instance()->writeRegister16b(REG_RW_MOT_0_PWM_H + 2 * port, adjustedSpeed);
}

void Motor::setPwmDirection(int port, Motor::Direction dir)
{
	if (port < 0 || port > 3) return;

	setControlMode(port, Private::Motor::Inactive);

	unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS);

	unsigned short offset = 2 * port;

	dirs &= ~(0x3 << offset);

	dirs |= (dir << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_MOT_DIRS, dirs);
}

unsigned short Motor::pwm(int port)
{
	if (port < 0 || port > 3) return 0;
	// TODO: error signal outside of range
	return Private::Wallaby::instance()->readRegister16b(REG_RW_MOT_0_PWM_H + 2 * port);
}

Motor::Direction Motor::pwmDirection(int port) const
{
	if (port < 0 || port > 3) return Private::Motor::Direction::PassiveStop;
	// TODO: error signal outside of range

	unsigned char dirs = Private::Wallaby::instance()->readRegister8b(REG_RW_MOT_DIRS);

	unsigned short offset = 2 * port;

	return (Motor::Direction)((dirs & (0x3 << offset)) >> offset);
}

void Motor::stop(int port)
{
	if (port < 0 || port > 3) return;
	setControlMode(port, Private::Motor::Inactive);
	setPwmDirection(port, PassiveStop);
}

int Motor::backEMF(int port)
{
	if (port < 0 || port > 3) return 0;
	return 0; // TODO: can we read this?
}

Motor * Motor::instance()
{
	static Motor instance;
	return & instance;
}

Motor::Motor()
{

}

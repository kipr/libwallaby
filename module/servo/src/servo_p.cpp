#include "servo_p.hpp"

#include "kipr/servo/servo.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/core/cleanup.hpp"

#include <cmath>
#include <thread>

using namespace kipr;
using namespace kipr::servo;

using kipr::core::Platform;
using kipr::core::cleanup_mutex;

void kipr::servo::set_servo_enabled(int port, bool enabled)
{
  if (port > 3) return;

  unsigned short allStop = Platform::instance()->readRegister8b(REG_RW_MOT_SRV_ALLSTOP);

  const unsigned short bit = 1 << (port + 4);

  if (!enabled)
  {
    allStop |= bit;
  }
  else
  {
    allStop &= ~bit;
  }

  if(enabled) std::lock_guard<std::mutex> lock(cleanup_mutex);
  Platform::instance()->writeRegister8b(REG_RW_MOT_SRV_ALLSTOP, allStop);
}


bool kipr::servo::get_servo_enabled(int port)
{
  if (port > 3) return false;

  unsigned short allStop = Platform::instance()->readRegister8b(REG_RW_MOT_SRV_ALLSTOP);

  const unsigned short bit = 1 << (port + 4);
  const bool currentlyDisabled = allStop & bit;

  return !currentlyDisabled;
}


bool kipr::servo::set_servo_position(int port, unsigned short position)
{
  if (port > 3) return false;
  if (position > 2047) position = 2047;

  // map a 10 bit (0-2047) position to  1500 +/- (0 to 90) degrees*10
  // or    1500 +/- 900  or  [600, 2400]
  unsigned short val =  1500 + std::round(1800.0 * ((double)position / 2047.0)) - (1800 / 2);

  unsigned char address = REG_RW_SERVO_0_H + 2 * port;
  std::lock_guard<std::mutex> lock(cleanup_mutex);
  Platform::instance()->writeRegister16b(address, val);
  return true;
}


unsigned short kipr::servo::get_servo_position(int port)
{
  if (port > 3) return 0xFFFF;

  unsigned char address = REG_RW_SERVO_0_H + 2 * port;
  const unsigned short position = Platform::instance()->readRegister16b(address);

  double degrees = ((double)position - 1500.0) / 10.0; // [-90, 90]
  double dval = (degrees + 90.0)  * 2047.0 / 180.0; // [0, 2047]

  if (dval < 0.0) dval = 0.0;
  if (dval > 2047.0) dval = 2047.01;
  unsigned short val = static_cast<unsigned short>(dval + 0.5);

  return val;
}

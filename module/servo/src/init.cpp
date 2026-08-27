#include "kipr/servo/servo.h"
#include "kipr/core/cleanup.hpp"

namespace
{
  __attribute__((constructor(2)))
  void register_servo_cleanup()
  {
    kipr::core::cleanup_add(disable_servos);
  }
}

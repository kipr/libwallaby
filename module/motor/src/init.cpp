#include "kipr/motor/motor.h"
#include "kipr/core/cleanup.hpp"

namespace
{
  __attribute__((constructor))
  void register_motor_cleanup()
  {
    kipr::core::cleanup_add(ao);
  }
}

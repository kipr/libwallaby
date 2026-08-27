#ifndef _KIPR_MOTOR_COMMAND_HPP_
#define _KIPR_MOTOR_COMMAND_HPP_

#include "kipr/core/command.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/motor/constants.hpp"


#include <array>

#include <cstdint>

namespace kipr
{
  namespace motor
  {
    namespace command
    {
      enum class Mode : std::uint8_t
      {
        Inactive = 0,
        Speed,
        Position,
        SpeedPosition
      };

      enum class Direction : std::uint8_t
      {
        PassiveStop = 0,
        Forward,
        Reverse,
        ActiveStop
      };

      core::Command set_modes(core::Memory &memory, const Mode mode);
      core::Command set_mode(core::Memory &memory, const std::uint8_t port, const Mode mode);

      core::Command set_directions(core::Memory &memory, const Direction direction);
      core::Command set_direction(core::Memory &memory, const std::uint8_t port, const Direction direction);

      std::array<core::Command, 2> all_off(core::Memory &memory);
    }


  }
}

#endif

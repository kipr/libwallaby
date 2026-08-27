#include "kipr/motor/command.hpp"
#include <stdexcept>

using namespace kipr;
using namespace kipr::motor;
using namespace kipr::motor::command;

using kipr::core::Command;
using kipr::core::Memory;
using kipr::core::w8;
using std::uint8_t;

Command command::set_modes(Memory &memory, const Mode mode)
{
  return w8(
    REG_RW_MOT_MODES,
    memory.u8(
      (static_cast<uint8_t>(mode) << 6) |
      (static_cast<uint8_t>(mode) << 4) |
      (static_cast<uint8_t>(mode) << 2) |
      (static_cast<uint8_t>(mode) << 0)
    )
  );
}

Command command::set_mode(Memory &memory, const std::uint8_t port, const Mode mode)
{
  if (port >= MOTOR_COUNT) throw std::out_of_range("Port is greater than motor count");

  const uint8_t offset = port * 2;

  return w8(
    REG_RW_MOT_MODES,
    memory.u8(static_cast<uint8_t>(mode) << offset),
    0x3 << offset
  );
}

core::Command command::set_directions(core::Memory &memory, const Direction direction)
{
  return w8(
    REG_RW_MOT_DIRS,
    memory.u8(
      (static_cast<uint8_t>(direction) << 6) |
      (static_cast<uint8_t>(direction) << 4) |
      (static_cast<uint8_t>(direction) << 2) |
      (static_cast<uint8_t>(direction) << 0)
    )
  );
}

core::Command command::set_direction(core::Memory &memory, const std::uint8_t port, const Direction direction)
{
  if (port >= MOTOR_COUNT) throw std::out_of_range("Port is greater than motor count");

  const uint8_t offset = port * 2;

  return w8(
    REG_RW_MOT_DIRS,
    memory.u8(static_cast<uint8_t>(direction) << offset),
    0x3 << offset
  );
}

std::array<Command, 2> command::all_off(core::Memory &memory)
{
  return {
    set_modes(memory, Mode::Inactive),
    set_directions(memory, Direction::PassiveStop)
  };
}

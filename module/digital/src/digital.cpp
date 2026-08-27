#include "kipr/digital/digital.hpp"
#include "digital_p.hpp"

using namespace kipr;
using namespace kipr::digital;

Digital::Digital(unsigned char port)
  : m_port(port)
{
}

void Digital::setValue(bool value)
{
  set_digital_value(m_port, value);
}

bool Digital::value() const
{
  return digital_value(m_port);
}

void Digital::setOutput(bool output)
{
  set_digital_direction(m_port, output);
}

bool Digital::isOutput() const
{
  return digital_output(m_port);
}

void Digital::setPullup(bool pullup)
{
  // not available on the Wallaby
}

bool Digital::pullup() const
{
  // not available on the Wallaby
  return false;
}

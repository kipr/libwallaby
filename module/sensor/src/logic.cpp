#include "kipr/sensor/logic.hpp"

using namespace kipr;
using namespace kipr::sensor;
using namespace kipr::sensor::logic;

Base::Base(const Sensor<bool> *a, const Sensor<bool> *b, bool owns)
  : m_a(a)
  , m_b(b)
  , m_owns(owns)
{
}

Base::~Base()
{
  if(!m_owns) return;
  delete m_a;
  delete m_b;
}

const Sensor<bool> *Base::a() const
{
  return m_a;
}

const Sensor<bool> *Base::b() const
{
  return m_b;
}

bool Base::owns() const
{
  return m_owns;
}

And::And(const Sensor<bool> *a, const Sensor<bool> *b, bool owns)
  : Base(a, b, owns)
{
}

bool And::value() const
{
  return a()->value() && b()->value();
}

Or::Or(const Sensor<bool> *a, const Sensor<bool> *b, bool owns)
  : Base(a, b, owns)
{
}

bool Or::value() const
{
  return a()->value() || b()->value();
}

Xor::Xor(const Sensor<bool> *a, const Sensor<bool> *b, bool owns)
  : Base(a, b, owns)
{
}

bool Xor::value() const
{
  const bool b = Base::b()->value();
  return a()->value() ? !b : b;
}

Not::Not(const Sensor<bool> *input, bool owns)
  : m_input(input),
  m_owns(owns)
{
}

Not::~Not()
{
  if(!m_owns) return;
  delete m_input;
}

bool Not::value() const
{
  return !m_input->value();
}

const Sensor<bool> *Not::input() const
{
  return m_input;
}

bool Not::owns() const
{
  return m_owns;
}

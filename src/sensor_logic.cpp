/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "wallaby/sensor_logic.hpp"

using namespace SensorLogic;

Base::Base(const Sensor<bool> *a, const Sensor<bool> *b, bool owns)
	: m_a(a),
	m_b(b),
	m_owns(owns)
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

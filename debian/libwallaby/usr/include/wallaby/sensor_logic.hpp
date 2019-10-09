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

#ifndef _SENSOR_LOGIC_HPP_
#define _SENSOR_LOGIC_HPP_

#include "sensor.hpp"
#include "export.h"

/*!
 * Contains all logical sensors (sensors apply logical operations to other sensors.)
 * \ingroup sensor
 */
namespace SensorLogic
{
	class EXPORT_SYM Base : public Sensor<bool>
	{
	public:
		Base(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
		~Base();
		
		const Sensor<bool> *a() const;
		const Sensor<bool> *b() const;
		bool owns() const;
	private:
		const Sensor<bool> *m_a;
		const Sensor<bool> *m_b;
		bool m_owns;
	};
	
	class EXPORT_SYM And : public Base
	{
	public:
		And(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
		virtual bool value() const;
	};
	
	class EXPORT_SYM Or : public Base
	{
	public:
		Or(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
		virtual bool value() const;
	};
	
	class EXPORT_SYM Xor : public Base
	{
	public:
		Xor(const Sensor<bool> *a, const Sensor<bool> *b, bool owns = false);
		virtual bool value() const;
	};
	
	class EXPORT_SYM Not : public Sensor<bool>
	{
	public:
		Not(const Sensor<bool> *input, bool owns = false);
		~Not();
		virtual bool value() const;
		
		const Sensor<bool> *input() const;
		bool owns() const;
		
	private:
		const Sensor<bool> *m_input;
		bool m_owns;
	};
}

#endif

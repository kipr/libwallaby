/*
 * geom.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GEOM_HPP_
#define INCLUDE_WALLABY_GEOM_HPP_



#include "geom.h"
#include "export.h"

template<typename T>
class EXPORT_SYM Point2
{
public:
	Point2(const T& x, const T &y)
		: m_x(x),
		m_y(y)
	{
	}

	const T &x() const
	{
		return m_x;
	}

  const T &row() const
  {
    return m_y;
  }

	const T &y() const
	{
		return m_y;
	}

  const T &column() const
  {
    return m_x;
  }

	void setX(const T &x)
	{
		m_x = x;
	}

	void setColumn(const T &column)
	{
		m_x = column;
	}

	void setY(const T &y)
	{
		m_y = y;
	}

	void setRow(const T &row)
	{
		m_y = row;
	}

	point2 toCPoint2() const
	{
		return create_point2(m_x, m_y);
	}

private:
	T m_x;
	T m_y;
};

template<typename T>
class EXPORT_SYM Point3
{
public:
	Point3(const T& x, const T &y, const T &z)
		: m_x(x),
		m_y(y),
    m_z(z)
	{
	}

	const T &x() const
	{
		return m_x;
	}

	const T &y() const
	{
		return m_y;
	}

	const T &z() const
	{
		return m_z;
	}

	void setX(const T &x)
	{
		m_x = x;
	}

	void setY(const T &y)
	{
		m_y = y;
	}

	void setZ(const T &z)
	{
		m_z = z;
	}

	point3 toCPoint3() const
	{
		return create_point3(m_x, m_y, m_z);
	}

private:
	T m_x;
	T m_y;
	T m_z;
};

template<typename T>
class EXPORT_SYM Rect
{
public:
	Rect(const T &x, const T &y, const T &width, const T &height)
		: m_x(x),
		m_y(y),
		m_width(width),
		m_height(height)
	{
	}

	const T &x() const
	{
		return m_x;
	}

	const T &y() const
	{
		return m_y;
	}

	const T &width() const
	{
		return m_width;
	}

	const T &height() const
	{
		return m_height;
	}

	Point2<T> center() const
	{
		return Point2<T>(m_x + m_width / 2, m_y + m_height / 2);
	}

	void setX(const T &x)
	{
		m_x = x;
	}

	void setY(const T &y)
	{
		m_y = y;
	}

	void setWidth(const T &width)
	{
		m_width = width;
	}

	void setHeight(const T &height)
	{
		m_x = height;
	}

	T area() const
	{
		return m_width * m_height;
	}

	rectangle toCRectangle() const
	{
		return create_rectangle(m_x, m_y, m_width, m_height);
	}

private:
	T m_x;
	T m_y;
	T m_width;
	T m_height;
};



#endif /* INCLUDE_WALLABY_GEOM_HPP_ */

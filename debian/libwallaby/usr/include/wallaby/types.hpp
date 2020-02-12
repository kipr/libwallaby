/*
 * types.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_TYPES_HPP_
#define INCLUDE_WALLABY_TYPES_HPP_


template<typename T>
struct Vec3
{
	Vec3();
	Vec3(const T &x, const T &y, const T &z);

	T x;
	T y;
	T z;
};

template<typename T>
Vec3<T>::Vec3()
	: x(0.0f),
	y(0.0f),
	z(0.0f)
{
}


template<typename T>
Vec3<T>::Vec3(const T &x, const T &y, const T &z)
	: x(x),
	y(y),
	z(z)
{
}

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3lf;
typedef Vec3<int> Vec3d;
typedef Vec3<unsigned> Vec3u;



#endif /* INCLUDE_WALLABY_TYPES_HPP_ */

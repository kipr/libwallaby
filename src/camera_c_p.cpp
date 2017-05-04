/*
 * camera_c_p.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#include "camera_c_p.hpp"

Camera::Device *Private::DeviceSingleton::s_device = 0;

Camera::Device *Private::DeviceSingleton::instance()
{
	if(!s_device) s_device = new Camera::Device();
	return s_device;
}

/*
 * camera_c_p.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#include "camera_c_p.hpp"

Camera::Device *Private::DeviceSingleton::s_device = 0;
Camera::InputProvider *Private::DeviceSingleton::s_inputProvider = 0;

void Private::DeviceSingleton::setInputProvider(Camera::InputProvider *const inputProvider)
{
	delete s_inputProvider;
	s_inputProvider = inputProvider;

	delete s_device;
	s_device = 0;
}

Camera::Device *Private::DeviceSingleton::instance()
{
	if(!s_device) s_device = new Camera::Device(s_inputProvider);
	return s_device;
}

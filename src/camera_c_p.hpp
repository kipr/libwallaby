/*
 * camera_c_p.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_CAMERA_C_P_HPP_
#define SRC_CAMERA_C_P_HPP_


#include "wallaby/camera.hpp"

namespace Private
{
	class DeviceSingleton
	{
	public:
		static void setInputProvider(::Camera::InputProvider *const inputProvider);

		static ::Camera::Device *instance();

	private:
		static ::Camera::Device *s_device;
		static ::Camera::InputProvider *s_inputProvider;
	};
}



#endif /* SRC_CAMERA_C_P_HPP_ */

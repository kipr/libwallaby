#include "camera_c_p.hpp"

using namespace kipr;
using namespace kipr::camera;

Device *kipr::camera::DeviceSingleton::s_device = 0;

Device *kipr::camera::DeviceSingleton::instance()
{
	if(!s_device) s_device = new Device();
	return s_device;
}

/*
 * camera_c.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#include "wallaby/camera.h"
#include "wallaby/camera.hpp"
#include "nyi.h"
#include "camera_c_p.hpp"

#include <iostream>
#include <cstdlib>

using namespace Private;

VI int camera_open()
{
  return camera_open_at_res(LOW_RES);
}

VI int camera_open_at_res(enum Resolution res)
{
	DeviceSingleton::setInputProvider(new Camera::UsbInputProvider);
	bool ret = DeviceSingleton::instance()->open();
	if(!ret) return 0;
	int width = 0;
	int height = 0;
	switch(res) {
	case LOW_RES:
		width = 160;
		height = 120;
		break;
	case MED_RES:
		width = 320;
		height = 240;
		break;
	case HIGH_RES:
		width = 640;
		height = 480;
		break;
	default: break;
	}
	if(width) DeviceSingleton::instance()->setWidth(width);
	if(height) DeviceSingleton::instance()->setHeight(height);
	return 1;
}

VI int camera_open_device(int number, enum Resolution res)
{
	bool ret = DeviceSingleton::instance()->open(number);
	if(!ret) return 0;
	int width = 0;
	int height = 0;
	switch(res) {
	case LOW_RES:
		width = 160;
		height = 120;
		break;
	case MED_RES:
		width = 320;
		height = 240;
		break;
	case HIGH_RES:
		width = 640;
		height = 480;
		break;
	}
	DeviceSingleton::instance()->setWidth(width);
	DeviceSingleton::instance()->setHeight(height);
	return 1;
}

VI int camera_load_config(const char *name)
{
	Config *config = Config::load(Camera::ConfigPath::path(name));
	if(!config) return 0;
	DeviceSingleton::instance()->setConfig(*config);
	delete config;
	return 1;
}

VI void set_camera_width(int width)
{
	if(width <= 0) {
		std::cout << "Camera width must be greater than 0." << std::endl;
		return;
	}
	DeviceSingleton::instance()->setWidth(width);
}

VI void set_camera_height(int height)
{
	if(height <= 0) {
		std::cout << "Camera height must be greater than 0." << std::endl;
		return;
	}
	DeviceSingleton::instance()->setHeight(height);
}

VI int get_camera_width(void)
{
	return DeviceSingleton::instance()->width();
}

VI int get_camera_height(void)
{
	return DeviceSingleton::instance()->height();
}

VI int camera_update(void)
{
	return DeviceSingleton::instance()->update() ? 1 : 0;
}

VI pixel get_camera_pixel(point2 p)
{
	const cv::Mat &mat = DeviceSingleton::instance()->rawImage();
	if(mat.empty()) {
		std::cout << "Camera image is empty." << std::endl;
		return pixel();
	}

	if(p.x < 0 || p.y < 0 || p.x > mat.cols || p.y > mat.rows) {
		std::cout << "Point isn't within the image." << std::endl;
		return pixel();
	}

	const cv::Vec3b v = mat.at<cv::Vec3b>(p.y, p.x);

	pixel ret;
	ret.r = v[2];
	ret.g = v[1];
	ret.b = v[0];

	return ret;
}

VI int get_channel_count(void)
{
	return DeviceSingleton::instance()->channels().size();
}

bool check_channel(int i)
{
	const Camera::ChannelPtrVector &channels = DeviceSingleton::instance()->channels();
	if(i < 0 || i >= channels.size()) {
		std::cout << "Channel must be in the range 0 .. " << (channels.size() - 1) << std::endl;
		return false;
	}
	return true;
}

bool check_channel_and_object(int i, int j)
{
	const Camera::ChannelPtrVector &channels = DeviceSingleton::instance()->channels();
	if(i < 0 || i >= channels.size()) {
		if(!channels.size()) std::cout << "Active configuration doesn't have any channels.";
		else std::cout << "Channel must be in the range 0 .. " << (channels.size() - 1);
		std::cout << std::endl;
		return false;
	}
	const Camera::ObjectVector *objs = channels[i]->objects();
	if(j < 0 || j >= objs->size()) {
		std::cout << "No such object " << j << std::endl;
		return false;
	}
	return true;
}

VI int get_object_count(int channel)
{
	if(!check_channel(channel)) return -1;
	return DeviceSingleton::instance()->channels()[channel]->objects()->size();
}

VI double get_object_confidence(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0.0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.confidence();
}


VI const char *get_object_data(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.data();
}

VI int get_code_num(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return -1;
	const char *data = get_object_data(channel, object);
	if(!data) return 0;
	return atoi(data);
}

VI int get_object_data_length(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return 0;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.dataLength();
}

VI int get_object_area(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return -1;
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.boundingBox().area();
}

VI rectangle get_object_bbox(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return create_rectangle(-1, -1, 0, 0);
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.boundingBox().toCRectangle();
}

VI int get_object_bbox_ulx(int channel, int object)
{
  return get_object_bbox(channel, object).ulx;
}

VI int get_object_bbox_uly(int channel, int object)
{
  return get_object_bbox(channel, object).uly;
}

VI int get_object_bbox_brx(int channel, int object)
{
  const rectangle r = get_object_bbox(channel, object);
  return r.ulx + r.width;
}

VI int get_object_bbox_bry(int channel, int object)
{
  const rectangle r = get_object_bbox(channel, object);
  return r.uly + r.height;
}

VI int get_object_bbox_width(int channel, int object)
{
  return get_object_bbox(channel, object).width;
}

VI int get_object_bbox_height(int channel, int object)
{
  return get_object_bbox(channel, object).height;
}

VI point2 get_object_centroid(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return create_point2(-1, -1);
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.centroid().toCPoint2();
}

VI int get_object_centroid_column(int channel, int object)
{
  return get_object_centroid(channel, object).x;
}

VI int get_object_centroid_x(int channel, int object)
{
  return get_object_centroid(channel, object).x;
}

VI int get_object_centroid_row(int channel, int object)
{
  return get_object_centroid(channel, object).y;
}

VI int get_object_centroid_y(int channel, int object)
{
  return get_object_centroid(channel, object).y;
}

VI point2 get_object_center(int channel, int object)
{
	if(!check_channel_and_object(channel, object)) return create_point2(-1, -1);
	const Camera::Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
	return o.boundingBox().center().toCPoint2();
}

VI int get_object_center_column(int channel, int object)
{
  return get_object_center(channel, object).x;
}

VI int get_object_center_x(int channel, int object)
{
  return get_object_center(channel, object).x;
}

VI int get_object_center_row(int channel, int object)
{
  return get_object_center(channel, object).y;
}

VI int get_object_center_y(int channel, int object)
{
  return get_object_center(channel, object).y;
}

VI void camera_close()
{
	DeviceSingleton::instance()->close();
}

VI void set_camera_config_base_path(const char *const path)
{
	Camera::ConfigPath::setBasePath(path);
}

VI const unsigned char *get_camera_frame_row(unsigned row)
{
	return DeviceSingleton::instance()->rawImage().ptr(row);
}

VI const unsigned char *get_camera_frame()
{
	return DeviceSingleton::instance()->bgr();
}

VI unsigned get_camera_element_size()
{
	return DeviceSingleton::instance()->rawImage().elemSize();
}



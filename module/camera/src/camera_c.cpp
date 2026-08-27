#include "kipr/camera/camera.h"
#include "kipr/camera/camera.hpp"
#include "kipr/camera/channel.hpp"
#include "kipr/camera/channel_impl.hpp"
#include "kipr/log/log.hpp"
#include "camera_c_p.hpp"
#include "logger.hpp"

#include <opencv2/core/core.hpp>

#include <iostream>
#include <cstdlib>

using namespace kipr;
using namespace kipr::camera;

using kipr::config::Config;

int camera_open()
{  
  return camera_open_at_res(HIGH_RES);
}

int camera_open_black()
{
	return camera_open_device_model_at_res(0, BLACK_2017, HIGH_RES);
}

int camera_open_at_res(enum Resolution res)
{
  return camera_open_device(0, res);
}

int camera_open_device(int number, enum Resolution res)
{
  int ret = camera_open_device_model_at_res(number, BLACK_2017 /*WHITE_2016*/, res);
  return ret;
}

int camera_open_device_model_at_res(int number, enum Model model, enum Resolution res)
{
  bool ret = DeviceSingleton::instance()->open(number, res, model);
  if(!ret) return 0;

  return 1;
}

int camera_load_config(const char *name)
{
  Config *config = Config::load(ConfigPath::path(name));
  if(!config) return 0;
  DeviceSingleton::instance()->setConfig(*config);
  delete config;
  return 1;
}

int get_camera_width(void)
{
  return DeviceSingleton::instance()->width();
}

int get_camera_height(void)
{
  return DeviceSingleton::instance()->height();
}

int camera_update(void)
{
  return DeviceSingleton::instance()->update() ? 1 : 0;
}

pixel get_camera_pixel(point2 p)
{
  const Image image = DeviceSingleton::instance()->rawImage();
  if(image.isEmpty()) {
    logger.error() << "camera image is empty";
    return pixel();
  }
  
  if(p.x < 0 || p.y < 0 || p.x >= image.getWidth() || p.y >= image.getHeight()) {
    logger.error() << "point isn't within the image";
    return pixel();
  }

  switch (image.getType())
  {
    case Image::Type::Bgr888:
    {
      const unsigned char *const data = image.getData() + p.y * image.getStride() + p.x * 3;
      pixel ret;
      ret.r = data[2];
      ret.g = data[1];
      ret.b = data[0];
      return ret;
    }
    case Image::Type::Rgb888:
    {
      const unsigned char *const data = image.getData() + p.y * image.getStride() + p.x * 3;
      pixel ret;
      ret.r = data[0];
      ret.g = data[1];
      ret.b = data[2];
      return ret;
    }
    case Image::Type::Grey8:
    {
      logger.error() << "grey8 not supported";
      return pixel();
    }
  }
}

int get_channel_count(void)
{
  return DeviceSingleton::instance()->channels().size();
}

bool check_channel(int i)
{
  const auto &channels = DeviceSingleton::instance()->channels();
  if(i < 0 || i >= channels.size()) {
    logger.error() << "Channel must be in the range 0 .. " << (channels.size() - 1);
    return false;
  }
  return true;
}

bool check_channel_and_object(int i, int j)
{
  const auto &channels = DeviceSingleton::instance()->channels();
  if(i < 0 || i >= channels.size()) {
    if(!channels.size()) logger.error() << "Active configuration doesn't have any channels.";
    else logger.error() << "Channel must be in the range 0 .. " << (channels.size() - 1);
    return false;
  }
  const auto *objs = channels[i]->objects();
  if(j < 0 || j >= objs->size()) {
    logger.error() << "No such object " << j;
    return false;
  }
  return true;
}

int get_object_count(int channel)
{
  if(!check_channel(channel)) return -1;
  return DeviceSingleton::instance()->channels()[channel]->objects()->size();
}

double get_object_confidence(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return 0.0;
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.confidence();
}


const char *get_object_data(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return 0;
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.data();
}

int get_code_num(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return -1;
  const char *data = get_object_data(channel, object);
  if(!data) return 0;
  return atoi(data);
}

int get_object_data_length(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return 0;
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.dataLength();
}

int get_object_area(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return -1;
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.boundingBox().area();
}

rectangle get_object_bbox(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return create_rectangle(-1, -1, 0, 0);
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.boundingBox().toCRectangle();
}

int get_object_bbox_ulx(int channel, int object)
{
  return get_object_bbox(channel, object).ulx;
}

int get_object_bbox_uly(int channel, int object)
{
  return get_object_bbox(channel, object).uly;
}

int get_object_bbox_brx(int channel, int object)
{
  const rectangle r = get_object_bbox(channel, object);
  return r.ulx + r.width;
}

int get_object_bbox_bry(int channel, int object)
{
  const rectangle r = get_object_bbox(channel, object);
  return r.uly + r.height;
}

int get_object_bbox_width(int channel, int object)
{
  return get_object_bbox(channel, object).width;
}

int get_object_bbox_height(int channel, int object)
{
  return get_object_bbox(channel, object).height;
}

point2 get_object_centroid(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return create_point2(-1, -1);
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.centroid().toCPoint2();
}

int get_object_centroid_column(int channel, int object)
{
  return get_object_centroid(channel, object).x;
}

int get_object_centroid_x(int channel, int object)
{
  return get_object_centroid(channel, object).x;
}

int get_object_centroid_row(int channel, int object)
{
  return get_object_centroid(channel, object).y;
}

int get_object_centroid_y(int channel, int object)
{
  return get_object_centroid(channel, object).y;
}

point2 get_object_center(int channel, int object)
{
  if(!check_channel_and_object(channel, object)) return create_point2(-1, -1);
  const Object &o = (*DeviceSingleton::instance()->channels()[channel]->objects())[object];
  return o.boundingBox().center().toCPoint2();
}

int get_object_center_column(int channel, int object)
{
  return get_object_center(channel, object).x;
}

int get_object_center_x(int channel, int object)
{
  return get_object_center(channel, object).x;
}

int get_object_center_row(int channel, int object)
{
  return get_object_center(channel, object).y;
}

int get_object_center_y(int channel, int object)
{
  return get_object_center(channel, object).y;
}

void camera_close()
{
  DeviceSingleton::instance()->close();
}

void set_camera_config_base_path(const char *const path)
{
  ConfigPath::setBasePath(path);
}

const unsigned char *get_camera_frame_row(unsigned row)
{
  const Image image = DeviceSingleton::instance()->rawImage();
  return image.getData() + row * image.getStride();
}

const unsigned char *get_camera_frame()
{
  return DeviceSingleton::instance()->bgr();
}

unsigned get_camera_element_size()
{
  
  const Image image = DeviceSingleton::instance()->rawImage();
  switch (image.getType())
  {
    case Image::Type::Rgb888: return 3;
    case Image::Type::Bgr888: return 3;
    case Image::Type::Grey8: return 1;
  }
  logger.error() << "Unsupported image type";
  return 0;
}

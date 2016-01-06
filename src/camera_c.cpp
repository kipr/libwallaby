/*
 * camera_c.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: Nafis Zaman
 */

#include "wallaby/camera.h"
#include "wallaby/camera.hpp"
#include "boyd_p.hpp"

#include <iostream>

int camera_open()
{
  return camera_open_at_res(LOW_RES);
}

int camera_open_at_res(enum Resolution res)
{
  return camera_open_device(0, res);
}

int camera_open_device(int number, enum Resolution res)
{
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
    default:
      std::cout << "Invalid resolution" << std::endl;
      return 0;
  }
  
  const bool result = Private::Camera::instance()->open(number, width, height);
  return result ? 1 : 0;
}

int camera_load_config(const char *name)
{
  Private::Camera::instance()->loadConfig(name);
  
  return 1;
}

int camera_set_width(int width)
{
  Private::Camera::instance()->setResolution(width, -1);
  
  return 1;
}

int camera_set_height(int height)
{
  Private::Camera::instance()->setResolution(-1, height);
  
  return 1;
}

// TODO: New function?
int camera_set_resolution(int width, int height)
{
  Private::Camera::instance()->setResolution(width, height);
  
  return 1;
}

int get_camera_width(void)
{
  return Private::Camera::instance()->width();
}

int get_camera_height(void)
{
  return Private::Camera::instance()->height();
}

int camera_update(void)
{
  const bool result = Private::Camera::instance()->update();
  return result ? 1 : 0;
}

pixel get_camera_pixel(point2 p)
{
  const std::vector<uint8_t> *const rawPixels = Private::Camera::instance()->rawPixels();
  // Check if we have a frame
  if(!rawPixels) {
    std::cout << "No frame available." << std::endl;
    return pixel();
  }
  
  const int w = get_camera_width();
  const int h = get_camera_height();
  // Check point bounds
  if(p.x < 0 || p.x >= w || p.y < 0 || p.y >= h) {
    std::cout << "Point is not within the image." << std::endl;
    return pixel();
  }
  
  // y * width * 3 + x * 3
  const int startIndex = (p.y * w + p.x) * 3;
  pixel ret;
  ret.b = rawPixels->at(startIndex);
  ret.g = rawPixels->at(startIndex+1);
  ret.r = rawPixels->at(startIndex+2);
  
  return ret;
}

int get_channel_count(void)
{
  const std::vector<std::vector<Object>> *const allChannelObjs = Private::Camera::instance()->channelBlobs();
  // Check if we have a frame
  if(!allChannelObjs) {
    std::cout << "No frame available." << std::endl;
    return 0;
  }
  return allChannelObjs->size();
}

int get_object_count(int channel)
{
  const std::vector<std::vector<Object>> *const allChannelObjs = Private::Camera::instance()->channelBlobs();
  // Check if we have a frame
  if(!allChannelObjs) {
    std::cout << "No frame available." << std::endl;
    return -1;
  }
  
  // Check channel bounds
  if(channel < 0 || channel >= allChannelObjs->size()) {
    std::cout << "Channel number is not valid." << std::endl;
    return -1;
  }
    
  return allChannelObjs->at(channel).size();
}

// TODO: Implement?
const char *get_object_data(int channel, int object)
{
  std::cout << "get_object_data is not implemented" << std::endl;
  return 0;
}

// TODO: Implement?
int get_code_num(int channel, int object)
{
  std::cout << "get_code_num is not implemented" << std::endl;
  return -1;
}

// TODO: Implement?
int get_object_data_length(int channel, int object)
{
  std::cout << "get_object_data_length is not implemented" << std::endl;
  return -1;
}

int get_object_confidence(int channel, int object)
{ 
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return -1;
  
  return Private::Camera::instance()->channelBlobs()->at(channel).at(object).confidence();
}

rectangle get_object_bbox(int channel, int object)
{
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return create_rectangle(-1, -1, 0, 0);
  
  return Private::Camera::instance()->channelBlobs()->at(channel).at(object).boundingBox().toCRectangle();
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

int get_object_area(int channel, int object)
{
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return -1;
  
  return Private::Camera::instance()->channelBlobs()->at(channel).at(object).boundingBox().area();
}

point2 get_object_centroid(int channel, int object)
{
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return create_point2(-1, -1);
  
  return Private::Camera::instance()->channelBlobs()->at(channel).at(object).centroid().toCPoint2();
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
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return create_point2(-1, -1);
  
  return Private::Camera::instance()->channelBlobs()->at(channel).at(object).boundingBox().center().toCPoint2();
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

// TODO: Change to camera_disconnect?
void camera_close()
{
  Private::Camera::instance()->close();
}

// TODO: Change to camera_set_config_base_path?
void set_camera_config_base_path(const char *const path)
{
  Private::Camera::instance()->setConfigBasePath(path);
}

// TODO: Do we need this? It's more work and a potential leak for the user
const unsigned char *get_camera_frame_row(unsigned row)
{
  std::cout << "get_camera_frame_row is not implemented" << std::endl;
  return 0;
}

const unsigned char *get_camera_frame()
{
  const std::vector<uint8_t> *const rawPixels = Private::Camera::instance()->rawPixels();
  if(!rawPixels)
    return 0;
  
  // TODO: Return a copy of this array? Would be expensive
  return &rawPixels->at(0);
}

// TODO: Implement?
unsigned get_camera_element_size()
{
  std::cout << "get_camera_element_size is not implemented" << std::endl;
  return 0;
}
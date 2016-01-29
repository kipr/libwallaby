/*
 * camera_c.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#include "wallaby/camera.h"
#include "wallaby/camera.hpp"
#include "warn.hpp"
#include "nyi.h"
#include "camera_c_p.hpp"

#include <iostream>
#include <cstdlib>

using namespace Private;

int camera_open()
{  
  return camera_open_at_res(LOW_RES);
}

int camera_open_at_res(enum Resolution res)
{
  if(res != LOW_RES) {
    WARN("only LOW_RES is currently supported");
    return 0;
  }
  
  return camera_open_device(0, res);
}

int camera_open_device(int number, enum Resolution res)
{
  if(res != LOW_RES) {
    WARN("only LOW_RES is currently supported");
    return 0;
  }
  
  const bool ret = DeviceSingleton::instance()->open(number);
  if(!ret) return 0;
  
  const int width = 160;
  const int height = 120;
  DeviceSingleton::instance()->setWidth(width);
  DeviceSingleton::instance()->setHeight(height);
  
  return 1;
}

int camera_update(void)
{
  return DeviceSingleton::instance()->update() ? 1 : 0;
}

pixel get_camera_pixel(point2 p)
{
  const cv::Mat &mat = DeviceSingleton::instance()->rawImage();
  if(mat.empty()) {
    WARN("camera image is empty");
    return pixel();
  }
  
  if(p.x < 0 || p.y < 0 || p.x >= mat.cols || p.y >= mat.rows) {
    WARN("point isn't within the image");
    return pixel();
  }
  
  const cv::Vec3b v = mat.at<cv::Vec3b>(p.y, p.x);
  
  pixel ret;
  ret.r = v[2];
  ret.g = v[1];
  ret.b = v[0];
  
  return ret;
}

void camera_close()
{
  DeviceSingleton::instance()->close();
}

const unsigned char *get_camera_frame_row(unsigned row)
{
  return DeviceSingleton::instance()->rawImage().ptr(row);
}

const unsigned char *get_camera_frame()
{
  return DeviceSingleton::instance()->bgr();
}
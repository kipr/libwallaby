/*
 * camera.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: Nafis Zaman
 */

#include "wallaby/camera.hpp"
#include "wallaby/color.hpp"
#include "boyd_p.hpp"
#include "warn.hpp"

#include <iostream>

Object::Object(const Point2<unsigned> &centroid,
  const Rect<unsigned> &boundingBox,
  const unsigned confidence)
  : m_centroid(centroid),
  m_boundingBox(boundingBox),
  m_confidence(confidence)
{}

const Point2<unsigned> &Object::centroid() const
{
  return m_centroid;
}
const Rect<unsigned> &Object::boundingBox() const
{
  return m_boundingBox;
}
const unsigned Object::confidence() const
{
  return m_confidence;
}

bool Camera::open()
{
  return this->open(Resolution::LOW_RES);
}

bool Camera::open(const Resolution res)
{
  return this->open(0, res);
}

bool Camera::open(const int deviceNumber, const Resolution res)
{
  switch(res) {
    case LOW_RES:
      return Private::Camera::instance()->open(deviceNumber, Private::Camera::Resolution::LOW_RES);
    case MED_RES:
      return Private::Camera::instance()->open(deviceNumber, Private::Camera::Resolution::MED_RES);
    case HIGH_RES:
      return Private::Camera::instance()->open(deviceNumber, Private::Camera::Resolution::HIGH_RES);
    default:
      WARN("invalid resolution");
      return false;
  }
}

bool Camera::close()
{
  return Private::Camera::instance()->close();
}

bool Camera::update()
{
  const bool result = Private::Camera::instance()->update();
  if(!result)
    WARN("failed");
  
  return result;
}

void Camera::loadConfig(const std::string &name)
{
  Private::Camera::instance()->loadConfig(name);
}

void Camera::setConfigBasePath(const std::string &path)
{
  return Private::Camera::instance()->setConfigBasePath(path);
}

void Camera::setWidth(const int width)
{
  if(width <= 0) {
    WARN("width must be positive");
    return;
  }
  
  Private::Camera::instance()->setResolution(width, -1);
}

void Camera::setHeight(const int height)
{
  if(height <= 0) {
    WARN("height must be positive");
    return;
  }
  
  Private::Camera::instance()->setResolution(-1, height);
}

void Camera::setResolution(const int width, const int height)
{
  if(width <= 0 && height <= 0) {
    WARN("width or height must be positive");
    return;
  }
  
  Private::Camera::instance()->setResolution(width, height);
}

int Camera::width()
{
  const int width = Private::Camera::instance()->width();
  if(width <= 0)
    WARN("no camera frame available");
  
  return width;
}

int Camera::height()
{
  const int height = Private::Camera::instance()->height();
  if(height <= 0)
    WARN("no camera frame available");
  
  return height;
}

Rgb Camera::getPixelRgb(Point2<int> point)
{
  const std::vector<uint8_t> *const rawPixels = Private::Camera::instance()->rawPixels();
  // Check if we have a frame
  if(!rawPixels) {
    WARN("no camera frame available");
    return Rgb();
  }
  
  const int w = this->width();
  const int h = this->height();
  // Check point bounds
  if(point.x() < 0 || point.x() >= w) {
    WARN("x must be between 0 and %d", w-1);
    return Rgb();
  }
  if(point.y() < 0 || point.y() >= h) {
    WARN("y must be between 0 and %d", h-1);
    return Rgb();
  }
  
  // y * width * 3 + x * 3
  const int startIndex = (point.y() * w + point.x()) * 3;
  Rgb ret;
  ret.b = rawPixels->at(startIndex);
  ret.g = rawPixels->at(startIndex+1);
  ret.r = rawPixels->at(startIndex+2);
  
  return ret;
}

const std::vector<Object> *Camera::getObjects(int channel)
{
  if(!Private::Camera::instance()->hasValidFrame()) {
    WARN("no camera frame available");
    return NULL;
  }
  
  if(!Private::Camera::instance()->checkChannel(channel)) {
    WARN("invalid channel");
    return NULL;
  }
  
  return &Private::Camera::instance()->channelBlobs()->at(channel);
}

const Object *Camera::getObject(int channel, int object)
{
  if(!Private::Camera::instance()->hasValidFrame()) {
    WARN("no camera frame available");
    return NULL;
  }
  
  if(!Private::Camera::instance()->checkChannelObject(channel, object)) {
    WARN("invalid channel or object");
    return NULL;
  }
  
  return &Private::Camera::instance()->channelBlobs()->at(channel).at(object);
}

int Camera::getChannelCount()
{
  if(!Private::Camera::instance()->hasValidFrame()) {
    WARN("no camera frame available");
    return -1;
  }
  
  const std::vector<std::vector<Object>> *const allChannelObjs = Private::Camera::instance()->channelBlobs();
  return allChannelObjs->size();
}

int Camera::getObjectCount(int channel)
{
  if(!Private::Camera::instance()->hasValidFrame()) {
    WARN("no camera frame available");
    return -1;
  }
  
  if(!Private::Camera::instance()->checkChannel(channel)) {
    WARN("invalid channel");
    return -1;
  }
  
  const std::vector<Object> *channelObjs = this->getObjects(channel);  
  return channelObjs->size();
}
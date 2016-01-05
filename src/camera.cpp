#include "wallaby/camera.hpp"
#include "wallaby/color.hpp"
#include "boyd_p.hpp"

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
  // TODO: Keep default resolution somewhere
  return this->open(480, 360);
}

bool Camera::open(const int width, const int height)
{
  return this->open(0, width, height);
}

bool Camera::open(const int deviceNumber, const int width, const int height)
{
  return Private::Camera::instance()->open(deviceNumber, width, height);
}

bool Camera::close()
{
  return Private::Camera::instance()->close();
}

bool Camera::update()
{
  return Private::Camera::instance()->update();
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
  Private::Camera::instance()->setWidth(width);
}

void Camera::setHeight(const int height)
{
  Private::Camera::instance()->setHeight(height);
}

int Camera::width()
{
  return Private::Camera::instance()->width();
}

int Camera::height()
{
  return Private::Camera::instance()->height();
}

Rgb Camera::getPixelRgb(Point2<int> point)
{
  const std::vector<uint8_t> *const rawPixels = Private::Camera::instance()->rawPixels();
  // Check if we have a frame
  if(!rawPixels) {
    std::cout << "No frame available." << std::endl;
    return Rgb();
  }
  
  const int w = this->width();
  const int h = this->height();
  // Check point bounds
  if(point.x() < 0 || point.x() >= w || point.y() < 0 || point.y() >= h) {
    std::cout << "Point is not within the image." << std::endl;
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
  if(!Private::Camera::instance()->checkChannel(channel))
    return NULL;
  
  return &Private::Camera::instance()->channelBlobs()->at(channel);
}

const Object *Camera::getObject(int channel, int object)
{
  if(!Private::Camera::instance()->checkChannelObject(channel, object))
    return NULL;
  
  return &Private::Camera::instance()->channelBlobs()->at(channel).at(object);
}

int Camera::getChannelCount()
{
  const std::vector<std::vector<Object>> *const allChannelObjs = Private::Camera::instance()->channelBlobs();
  if(!allChannelObjs)
    return -1;
  
  return allChannelObjs->size();
}

int Camera::getObjectCount(int channel)
{
  const std::vector<Object> *channelObjs = this->getObjects(channel);
  if(!channelObjs)
    return -1;
  
  return channelObjs->size();
}
#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <vector>
#include <string>

#include "wallaby/geom.hpp"
#include "wallaby/color.hpp"

class Object
{
public:
  Object(const Point2<unsigned> &centroid,
          const Rect<unsigned> &boundingBox,
          const unsigned confidence);
  const Point2<unsigned> &centroid() const;
  const Rect<unsigned> &boundingBox() const;
  const unsigned confidence() const;

private:
  Point2<unsigned> m_centroid;
  Rect<unsigned> m_boundingBox;
  unsigned m_confidence;
};

class Camera
{
public:
  bool open();
  bool open(const int width, const int height);
  bool open(const int deviceNumber, const int width, const int height);
  bool close();
  bool update();
  
  void loadConfig(const std::string &name);
  void setConfigBasePath(const std::string &path);
  
  void setWidth(const int width);
  void setHeight(const int height);
  int width();
  int height();
  
  Rgb getPixelRgb(Point2<int> point);
  const std::vector<Object> *getObjects(int channel);
  const Object *getObject(int channel, int object);
  int getChannelCount();
  int getObjectCount(int channel);
};

#endif

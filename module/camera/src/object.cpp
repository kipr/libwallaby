#include "kipr/camera/object.hpp"

#include <cstring>

using namespace kipr;
using namespace kipr::camera;

using kipr::geometry::Point2;
using kipr::geometry::Rect;

Object::Object(const Point2<unsigned> &centroid,
               const Rect<unsigned> &boundingBox,
               const double confidence, const char *data,
               const size_t &dataLength)
    : m_centroid(centroid),
      m_boundingBox(boundingBox),
      m_confidence(confidence),
      m_data(0),
      m_dataLength(dataLength)
{
  if (!data)
    return;

  m_data = new char[m_dataLength];
  memcpy(m_data, data, m_dataLength);
}

Object::Object(const Object &rhs)
    : m_centroid(rhs.m_centroid), m_boundingBox(rhs.m_boundingBox),
      m_confidence(rhs.m_confidence), m_data(0),
      m_dataLength(rhs.m_dataLength)
{
  if (!rhs.m_data)
    return;

  m_data = new char[m_dataLength + 1];
  memcpy(m_data, rhs.m_data, m_dataLength);
  m_data[m_dataLength] = 0;
}

Object::~Object() { delete[] m_data; }

const geometry::Point2<unsigned> &Object::centroid() const { return m_centroid; }

const geometry::Rect<unsigned> &Object::boundingBox() const
{
  return m_boundingBox;
}

const double Object::confidence() const { return m_confidence; }

const char *Object::data() const { return m_data; }

const size_t Object::dataLength() const { return m_dataLength; }
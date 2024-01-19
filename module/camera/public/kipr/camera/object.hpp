#ifndef _KIPR_CAMERA_OBJECT_HPP_
#define _KIPR_CAMERA_OBJECT_HPP_

#include "kipr/geometry/geometry.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>

namespace kipr
{
  namespace camera
  {
    class Object
    {
    public:
      Object(const geometry::Point2<unsigned> &centroid, const geometry::Rect<unsigned> &boundingBox,
            const double confidence, const char *data = 0,
            const size_t &dataLength = 0);
      Object(const Object &rhs);
      ~Object();

      const geometry::Point2<unsigned> &centroid() const;
      const geometry::Rect<unsigned> &boundingBox() const;

      const double confidence() const;
      const char *data() const;
      const size_t dataLength() const;

    private:
      geometry::Point2<unsigned> m_centroid;
      geometry::Rect<unsigned> m_boundingBox;
      double m_confidence;
      char *m_data;
      size_t m_dataLength;
    };

    typedef std::vector<Object> ObjectVector;
  }
}

#endif

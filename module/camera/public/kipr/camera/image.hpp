#ifndef _KIPR_CAMERA_IMAGE_HPP_
#define _KIPR_CAMERA_IMAGE_HPP_

#include <algorithm>

namespace kipr
{
  namespace camera
  {
    class Image
    {
    public:
      enum class Type
      {
        Grey8,
        Rgb888,
        Bgr888,
      };

      Image();

      Image(
        const Type type,
        const unsigned width,
        const unsigned height,
        const unsigned stride,
        unsigned char *const data,
        const bool owned = true
      );

      Image(const Image &);
      Image(Image &&);

      Image &operator=(Image);

      ~Image();

      bool isEmpty() const;

      Type getType() const;
      const unsigned char *const getData() const;

      unsigned getWidth() const;
      unsigned getHeight() const;
      unsigned getStride() const;
      bool isOwned() const;

      friend void swap(Image &first, Image &second)
      {
        using std::swap;
        swap(first.type_, second.type_);
        swap(first.data_, second.data_);
        swap(first.owned_, second.owned_);
        swap(first.width_, second.width_);
        swap(first.height_, second.height_);
        swap(first.stride_, second.stride_);
      }

    private:
      Type type_;
      unsigned char *data_;
      bool owned_;

      unsigned width_;
      unsigned height_;
      unsigned stride_;
    };
  }
}

#endif
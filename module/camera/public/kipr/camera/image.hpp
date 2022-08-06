#ifndef _KIPR_CAMERA_IMAGE_HPP_
#define _KIPR_CAMERA_IMAGE_HPP_

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

      ~Image();

      const unsigned char *const getData() const;
      unsigned getWidth() const;
      unsigned getHeight() const;
      unsigned getStride() const;
      bool isOwned() const;

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
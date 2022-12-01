#include "kipr/camera/image.hpp"

using namespace kipr;
using namespace kipr::camera;

Image::Image()
  : type_(Type::Grey8)
  , data_(nullptr)
  , owned_(false)
  , width_(0)
  , height_(0)
  , stride_(0)
{
}

Image::Image(
  const Type type,
  const unsigned width,
  const unsigned height,
  const unsigned stride,
  unsigned char *const data,
  const bool owned
) : type_(type)
  , data_(data)
  , owned_(owned)
  , width_(width)
  , height_(height)
  , stride_(stride)

{

}

Image::Image(const Image &rhs)
  : type_(rhs.type_)
  , data_(rhs.data_)
  , owned_(false)
  , width_(rhs.width_)
  , height_(rhs.height_)
  , stride_(rhs.stride_)
{
}

Image::Image(Image &&rhs)
  : type_(rhs.type_)
  , data_(rhs.data_)
  , owned_(rhs.owned_)
  , width_(rhs.width_)
  , height_(rhs.height_)
  , stride_(rhs.stride_)
{
  rhs.data_ = nullptr;
  rhs.owned_ = false;
}

Image &Image::operator=(Image other)
{
  swap(*this, other);
  return *this;
}

Image::~Image()
{
  if (owned_) delete[] data_;
}

bool Image::isEmpty() const { return data_ == nullptr; }
Image::Type Image::getType() const { return type_; }
const unsigned char *const Image::getData() const { return data_; }
unsigned Image::getWidth() const { return width_; }
unsigned Image::getHeight() const { return height_; }
unsigned Image::getStride() const { return stride_; }
bool Image::isOwned() const { return owned_; }
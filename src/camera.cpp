/*
 * camera.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#include "wallaby/camera.hpp"
#include "channel_p.hpp"
#include "warn.hpp"

#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <jpeglib.h>

#ifndef NOT_A_WALLABY
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#endif

using namespace Camera;

// Object //

Camera::Object::Object(const Point2<unsigned> &centroid,
  const Rect<unsigned> &boundingBox,
  const double confidence, const char *data,
  const size_t &dataLength)
  : m_centroid(centroid),
  m_boundingBox(boundingBox),
  m_confidence(confidence),
  m_data(0),
  m_dataLength(dataLength)
{
  if(!data) return;
  
  m_data = new char[m_dataLength];
  memcpy(m_data, data, m_dataLength);
}

Camera::Object::Object(const Object &rhs)
  : m_centroid(rhs.m_centroid),
  m_boundingBox(rhs.m_boundingBox),
  m_confidence(rhs.m_confidence),
  m_data(0),
  m_dataLength(rhs.m_dataLength)
{
  if(!rhs.m_data) return;
  
  m_data = new char[m_dataLength + 1];
  memcpy(m_data, rhs.m_data, m_dataLength);
  m_data[m_dataLength] = 0;
}

Camera::Object::~Object()
{
  delete[] m_data;
}

const Point2<unsigned> &Camera::Object::centroid() const
{
  return m_centroid;
}

const Rect<unsigned> &Camera::Object::boundingBox() const
{
  return m_boundingBox;
}

const double Camera::Object::confidence() const
{
  return m_confidence;
}

const char *Camera::Object::data() const
{
  return m_data;
}

const size_t Camera::Object::dataLength() const
{
  return m_dataLength;
}

ChannelImpl::ChannelImpl()
  : m_dirty(true)
{
}

ChannelImpl::~ChannelImpl()
{
}

void ChannelImpl::setImage(const cv::Mat &image)
{
  if(image.empty()) {
    m_image = cv::Mat();
    m_dirty = true;
    return;
  }
  m_image = image;
  m_dirty = true;
}

ObjectVector ChannelImpl::objects(const Config &config)
{
  if(m_dirty) {
    update(m_image);
    m_dirty = false;
  }
  return findObjects(config);
}

std::map<std::string, ChannelImpl *> ChannelImplManager::m_channelImpls = {{"hsv", new Private::Camera::HsvChannelImpl()}, {"qr", new Private::Camera::BarcodeChannelImpl}};

void ChannelImplManager::setImage(const cv::Mat &image)
{
  std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.begin();
  for(; it != m_channelImpls.end(); ++it) it->second->setImage(image);
}

ChannelImpl *ChannelImplManager::channelImpl(const std::string &name)
{
  std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.find(name);
  return (it == m_channelImpls.end()) ? 0 : it->second;
}

// Channel //

Camera::Channel::Channel(Device *device, const Config &config)
  : m_device(device),
  m_config(config),
  m_impl(0),
  m_valid(false)
{
  m_objects.clear();
  const std::string type = config.stringValue("type");
  if(type.empty()) {
    WARN("No type specified in config.");
    return;
  }
  
  m_impl = ChannelImplManager::channelImpl(type);
  if(!m_impl) {
    WARN("Type %s not found", type.c_str());
    return;
  }
}

Camera::Channel::~Channel()
{
}

void Camera::Channel::invalidate()
{
  m_valid = false;
}

struct AreaComparator
{
public:
  bool operator()(const Camera::Object &left, const Camera::Object &right)
  {
    return left.boundingBox().area() > right.boundingBox().area();
  }
} LargestAreaFirst;

const ObjectVector *Camera::Channel::objects() const
{
  if(!m_impl) return 0;
  if(!m_valid) {
    m_objects.clear();
    m_objects = m_impl->objects(m_config);
    std::sort(m_objects.begin(), m_objects.end(), LargestAreaFirst);
    m_valid = true;
  }
  return &m_objects;
}

Device *Camera::Channel::device() const
{
  return m_device;
}

void Camera::Channel::setConfig(const Config &config)
{
  m_config = config;
  invalidate();
}

// ConfigPath //

std::string Camera::ConfigPath::s_path = "/etc/botui/channels/";

std::string Camera::ConfigPath::extension()
{
  return "conf";
}

void Camera::ConfigPath::setBasePath(const std::string &path)
{
  s_path = path;
  if(s_path.empty()) return;
  if(s_path[s_path.size() - 1] != '/') s_path += "/";
}

std::string Camera::ConfigPath::path(const std::string &name)
{
  if(name.empty()) return s_path;
  return s_path + name + "." + extension();
}

std::string Camera::ConfigPath::defaultPath()
{
  return s_path + "default";
}

std::string Camera::ConfigPath::defaultConfigPath()
{
  std::ifstream file;
  file.open(defaultPath().c_str());
  if(!file.is_open()) return std::string();
  std::string ret;
  std::getline(file, ret);
  file.close();
  return ret;
}

void Camera::ConfigPath::setDefaultConfigPath(const std::string &name)
{
  std::ofstream file;
  file.open(defaultPath().c_str());
  if(!file.is_open()) return;
  const std::string path = ConfigPath::path(name);
  file.write(path.c_str(), path.size());
  file.close();  
}

// Device //

const char *Camera::Device::device_name = "/dev/video0";

Camera::Device::Device()
  : m_bgr(0),
  m_bgrSize(0),
  m_fd(-1)
{
  Config *config = Config::load(Camera::ConfigPath::defaultConfigPath());
  if(!config) return;
  setConfig(*config);
  delete config;
  
  // TODO: set initial resolution?
}

Camera::Device::~Device()
{
  ChannelPtrVector::const_iterator it = m_channels.begin();
  for(; it != m_channels.end(); ++it) delete *it;
  delete m_bgr;
}

bool Camera::Device::open(const int number, const unsigned width, const unsigned height)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return false;
#else
  // Device already open?
  if(this->isOpen()) return false;
  
  struct stat st;
  
  if(stat(device_name, &st) == -1) {
    fprintf(stderr, "Cannot identify '%s': %d, %s\n", device_name, errno, strerror(errno));
    return false;
  }
  
  if(!S_ISCHR(st.st_mode)) {
    fprintf(stderr, "%s is no device\n", device_name);
    return false;
  }
  
  m_fd = ::open(device_name, O_RDWR | O_NONBLOCK, 0);
  if(m_fd == -1) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", device_name, errno, strerror(errno));
    return false;
  }
  
  return this->initCapDevice(width, height);
#endif
}

bool Camera::Device::isOpen() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return false;
#else
  return (m_fd != -1);
#endif
}

void Camera::Device::setWidth(const unsigned width)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return;
#else
  WARN("setting width not supported");
#endif
}

void Camera::Device::setHeight(const unsigned height)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return;
#else
  WARN("setting height not supported");
#endif
}

unsigned Camera::Device::width() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return 0;
#else
  // TODO: Get actual camera width
  if(m_image.empty()) return 0;
  return m_image.cols;
#endif
}

unsigned Camera::Device::height() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return 0;
#else
  // TODO: Get actual camera height
  if(m_image.empty()) return 0;
  return m_image.rows;
#endif
}

bool Camera::Device::close()
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return false;
#else
  if(!this->isOpen()) return false;
  
  // Stop capturing
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(xioctl(m_fd, VIDIOC_STREAMOFF, &type) == -1) {
    // TODO: ignore this failure?
  }
  
  // Unmap buffers
  for(unsigned int i = 0; i < nBuffers; ++i)
    if(munmap(buffers[i].start, buffers[i].length) == -1) {
      // TODO: ignore this failure?
    }
  
  // Close device
  if(::close(m_fd) == -1) {
    // TODO: ignore this failure?
  }
  
  m_fd = -1;
  
  return true;
#endif
}

bool Camera::Device::update()
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return false;
#else  
  for(;;) {
    fd_set fds;    
    FD_ZERO(&fds);
    FD_SET(m_fd, &fds);
    
    // Timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    const int r = select(m_fd + 1, &fds, NULL, NULL, &tv);
    if(r == -1) {
      if(errno == EINTR) continue;
      m_image = cv::Mat();
      return false;
    }
    if(r == 0) {
      // Timed out
      m_image = cv::Mat();
      return false;
    }
    
    const int readRes = this->readFrame();
    if(readRes == -1) {
      // Something went wrong
      m_image = cv::Mat();
      return false;
    }
    else if(readRes == 1) {
      // Success
      break;
    }
  }
  
  // No need to update channels if there are none.
  if(m_channels.empty()) return true;
  
  // Dirty all channel impls
  ChannelImplManager::setImage(m_image);
  
  // Invalidate all channels
  ChannelPtrVector::const_iterator it = m_channels.begin();
  for(; it != m_channels.end(); ++it) (*it)->invalidate();
  return true;
#endif
}

const ChannelPtrVector &Camera::Device::channels() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
#endif
  
  return m_channels;
}

const cv::Mat &Camera::Device::rawImage() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
#endif
  
  return m_image;
}

void Camera::Device::setConfig(const Config &config)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return;
#else
  m_config = config;
  updateConfig();
#endif
}

const Config &Camera::Device::config() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
#endif
  
  return m_config;
}

const unsigned char *Camera::Device::bgr() const
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return 0;
#else
  const unsigned correctSize = m_image.rows * m_image.cols * m_image.elemSize();
  if(m_bgrSize != correctSize) {
    delete m_bgr;
    m_bgrSize = correctSize;
    m_bgr = new unsigned char[m_bgrSize];
  }
  
  for(unsigned row = 0; row < m_image.rows; ++row) {
    unsigned offset1 = row * m_image.cols * m_image.elemSize();
    memcpy(m_bgr + offset1, m_image.ptr(row), m_image.cols * m_image.elemSize());
  }
  
  return m_bgr;
#endif
}

void Camera::Device::updateConfig()
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return;
#else
  ChannelPtrVector::const_iterator it = m_channels.begin();
  for(; it != m_channels.end(); ++it) delete *it;
  m_channels.clear();
  
  m_config.clearGroup();
  m_config.beginGroup(CAMERA_GROUP);
  int numChannels = m_config.intValue(CAMERA_NUM_CHANNELS_KEY);
  if(numChannels <= 0) return;
  for(int i = 0; i < numChannels; ++i) {
    std::stringstream stream;
    stream << CAMERA_CHANNEL_GROUP_PREFIX;
    stream << i;
    m_config.beginGroup(stream.str());
    m_channels.push_back(new Channel(this, m_config));
    m_config.endGroup();
  }
  m_config.endGroup();
#endif
}

bool Camera::Device::initCapDevice(const unsigned width, const unsigned height)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return false;
#else
  if(!this->isOpen()) return false;
  
  // TODO: When this function fails, is calling close() enough? Other cleanup?
  
  struct v4l2_capability cap;
  if(xioctl(m_fd, VIDIOC_QUERYCAP, &cap) == -1) {
    this->close();
    return false;
  }
  
  if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
      || !(cap.capabilities & V4L2_CAP_STREAMING)) {
    this->close();
    return false;
  }
  
  // TODO: cropcap?
  
  // Set capture format properties
  struct v4l2_format fmt;
  memset(&fmt, 0, sizeof(fmt));
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = width;
  fmt.fmt.pix.height = height;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
  if(xioctl(m_fd, VIDIOC_S_FMT, &fmt) == -1) {
    this->close();
    return false;
  }
  
  // Set stream properties
  // TODO: Adjustable fps?
  struct v4l2_streamparm strm;
  memset(&strm, 0, sizeof(strm));
  strm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  strm.parm.capture.timeperframe.numerator = 1;
  strm.parm.capture.timeperframe.denominator = 4;
  if(xioctl(m_fd, VIDIOC_S_PARM, &strm) == -1) {
    this->close();
    return false;
  }
  
  // Buggy driver paranoia
  unsigned int min = fmt.fmt.pix.width * 2;
  if(fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if(fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;
  
  // Init mmap, request buffers
  struct v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if(xioctl(m_fd, VIDIOC_REQBUFS, &req) == -1 || req.count < 2) {
    this->close();
    return false;
  }
  
  buffers = (buffer *)calloc(req.count, sizeof(*buffers));
  if(!buffers) {
    this->close();
    return false;
  }
  
  for(nBuffers = 0; nBuffers < req.count; ++nBuffers) {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = nBuffers;
    if(xioctl(m_fd, VIDIOC_QUERYBUF, &buf) == -1) {
      this->close();
      return false;
    }
    
    buffers[nBuffers].length = buf.length;
    buffers[nBuffers].start =
            mmap(NULL /* start anywhere */,
                  buf.length,
                  PROT_READ | PROT_WRITE /* required */,
                  MAP_SHARED /* recommended */,
                  m_fd, buf.m.offset);

    if (buffers[nBuffers].start == MAP_FAILED) {
      this->close();
      return false;
    }
  }
  
  // Prepare for frame capturing
  for(unsigned int i = 0; i < nBuffers; ++i) {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if(xioctl(m_fd, VIDIOC_QBUF, &buf) == -1) {
      this->close();
      return false;
    }
  }
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(m_fd, VIDIOC_STREAMON, &type) == -1) {
    this->close();
    return false;
  }
  
  return true;
#endif
}

int Camera::Device::readFrame()
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return -1;
#else
  struct v4l2_buffer buf;
  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if(xioctl(m_fd, VIDIOC_DQBUF, &buf) == -1) {
    switch(errno) {
      case EAGAIN:
        // Try again later
        return 0;
      case EIO:
      default:
        // Something is wrong
        return -1;
    }
  }
  
  assert(buf.index < nBuffers);
  
  // Process frame
  // TODO: width and height shouldn't be hardcoded
  /*// OPENCV WAY
  cv::Mat jpgBuf(cv::Size(160, 120), CV_8UC3, buffers[buf.index].start);
  m_image = cv::imdecode(jpgBuf, CV_LOAD_IMAGE_COLOR);*/
  // LIBJPEG WAY
  m_image = this->decodeJpeg(buffers[buf.index].start, buf.bytesused);
  
  if(xioctl(m_fd, VIDIOC_QBUF, &buf) == -1)
    return -1;
  
  // Success!
  return 1;
#endif
}

METHODDEF(void) emit_message_suppressed(j_common_ptr cinfo, int msg_level)
{
}

cv::Mat Camera::Device::decodeJpeg(void *p, int size)
{
  if(size <= 0)
    return cv::Mat();
  
  // Init JPEG decompression objects
  struct jpeg_decompress_struct cInfo;
  struct jpeg_error_mgr jerr;
  cInfo.err = jpeg_std_error(&jerr);
  jerr.emit_message = emit_message_suppressed;
  jpeg_create_decompress(&cInfo);
  
  // Compressed data source
  jpeg_mem_src(&cInfo, (unsigned char *)p, size); // parameters?
  
  // Read JPEG header
  jpeg_read_header(&cInfo, true);
  const int width = cInfo.image_width;
  const int height = cInfo.image_height;
  const int numComp = cInfo.num_components;
  
  jpeg_start_decompress(&cInfo);
  const int outWidth = cInfo.output_width;
  const int outHeight = cInfo.output_height;
  const int outComp = cInfo.output_components;
  
  unsigned long bmp_size = outWidth * outHeight * outComp;
  unsigned char *bmp_buffer = (unsigned char *) malloc(bmp_size);
  int row_stride = width * outComp;
  
  while(cInfo.output_scanline < cInfo.output_height) {
    unsigned char *buffer_array[1];
    buffer_array[0] = bmp_buffer + (cInfo.output_scanline) * row_stride;
    jpeg_read_scanlines(&cInfo, buffer_array, 1);
  }
  
  free(bmp_buffer);

  jpeg_finish_decompress(&cInfo);
  jpeg_destroy_decompress (&cInfo);
    
  cv::Mat image(cv::Size(outWidth, outHeight), CV_8UC3, bmp_buffer);
  cv::cvtColor(image, image, CV_BGR2RGB);
  
  return image;
}

int Camera::Device::xioctl(int fh, int request, void *arg)
{
#ifdef NOT_A_WALLABY
  WARN("camera only supported on wallaby");
  return -1;
#else
  int r;
  do {
    r = ioctl(fh, request, arg);
  } while (-1 == r && EINTR == errno);
  
  return r;
#endif
}

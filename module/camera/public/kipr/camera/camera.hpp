/*
 * camera.hpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#ifndef _KIPR_CAMERA_CAMERA_HPP_
#define _KIPR_CAMERA_CAMERA_HPP_

#include "kipr/color/color.hpp"
#include "kipr/config/config.hpp"
#include "kipr/geometry/geometry.hpp"
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "kipr/camera/camera.h"

#include <opencv2/core/core.hpp>

// These keys are used in the config files loaded by
// Camera::Device
#define CAMERA_GROUP ("camera")
#define CAMERA_NUM_CHANNELS_KEY ("num_channels")
#define CAMERA_CHANNEL_GROUP_PREFIX ("channel_")
#define CAMERA_CHANNEL_TYPE_KEY ("type")

#define CAMERA_CHANNEL_TYPE_HSV_KEY ("hsv")
#define CAMERA_CHANNEL_TYPE_QR_KEY ("qr")
#define CAMERA_CHANNEL_TYPE_ARUCO_KEY ("aruco")

namespace cv
{
  class VideoCapture;
}

namespace kipr
{
  namespace camera
  {
    class Device;

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

    class ChannelImpl
    {
    public:
      ChannelImpl();
      virtual ~ChannelImpl();

      void setImage(const cv::Mat &image);
      ObjectVector objects(const config::Config &config);

    protected:
      virtual void update(const cv::Mat &image) = 0;
      virtual ObjectVector findObjects(const config::Config &config) = 0;

    private:
      bool m_dirty;
      cv::Mat m_image;
    };

    class ChannelImplManager
    {
    public:
      static void setImage(const cv::Mat &image);
      static ChannelImpl *channelImpl(const std::string &name);

    private:
      // TODO: private constructor?
      static std::map<std::string, ChannelImpl *> m_channelImpls;
    };

    class Channel
    {
    public:
      Channel(Device *device, const config::Config &config);
      ~Channel();

      void invalidate();
      const ObjectVector *objects() const;
      Device *device() const;

      /**
      * Do not call this method unless you know what you are doing!
      */
      void setConfig(const config::Config &config);

    private:
      Device *m_device;
      config::Config m_config;
      mutable ObjectVector m_objects;
      ChannelImpl *m_impl;
      mutable bool m_valid;
    };

    typedef std::vector<Channel *> ChannelPtrVector;

    class ConfigPath
    {
    public:
      static std::string extension();

      static void setBasePath(const std::string &path);
      static std::string path(const std::string &name = std::string());
      static std::string defaultPath();
      static std::string defaultConfigPath();
      static void setDefaultConfigPath(const std::string &name);

    private:
      static std::string s_path;
    };

    class Device
    {
    public:
      Device();
      ~Device();

      bool open(const int number = 0, Resolution resolution = LOW_RES,
                Model model = WHITE_2016);
      bool isOpen() const;
      bool close();
      bool update();

      unsigned width() const;
      unsigned height() const;

      static unsigned int resolutionToHeight(Resolution res);
      static unsigned int resolutionToWidth(Resolution res);

      const ChannelPtrVector &channels() const;

      const cv::Mat &rawImage() const;

      void setConfig(const config::Config &config);
      const config::Config &config() const;

      const unsigned char *bgr() const;

      Model getModel();

    private:
      void updateConfig();
      bool initCapDevice(const unsigned width, const unsigned height);
      int readFrame();
      cv::Mat decodeJpeg(void *p, int size);
      int xioctl(int fh, int request, void *arg);

      struct buffer
      {
        void *start;
        size_t length;
      };
      struct buffer *buffers;
      unsigned int nBuffers;

      // cv::VideoCapture *m_capture;
      config::Config m_config;
      ChannelPtrVector m_channels;
      cv::Mat m_image;
      unsigned char *m_bmpBuffer;

      mutable unsigned char *m_bgr;
      mutable unsigned m_bgrSize;

      int m_fd;
      static const char *device_name;
      cv::VideoCapture *m_cap;
      bool m_connected;

      Resolution m_resolution;
      Model m_model;

      unsigned int SelectTimeoutSec;
      unsigned int SelectTimeoutuSec;
    };

    /**
    * Retrieve the device singleton that backs the libkovan C camera API
    */
    Device *cDevice();
  }
}

#endif

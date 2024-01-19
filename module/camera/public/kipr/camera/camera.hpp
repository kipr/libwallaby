#ifndef _KIPR_CAMERA_CAMERA_HPP_
#define _KIPR_CAMERA_CAMERA_HPP_

#include "kipr/color/color.hpp"
#include "kipr/config/config.hpp"
#include "kipr/geometry/geometry.hpp"
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <memory>

#include "kipr/camera/camera.h"
#include "kipr/camera/image.hpp"


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
    class Channel;
    class DeviceImpl;

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

      const std::vector<Channel *> &channels() const;

      Image rawImage() const;

      void setConfig(const config::Config &config);
      const config::Config &config() const;

      const unsigned char *bgr() const;

      Model getModel();

    private:
      void updateConfig();
      bool initCapDevice(const unsigned width, const unsigned height);
      int readFrame();
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
      std::vector<Channel *> m_channels;
      std::unique_ptr<DeviceImpl> m_impl;

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

#ifndef _KIPR_CAMERA_CHANNEL_P_HPP_
#define _KIPR_CAMERA_CHANNEL_P_HPP_

#include "kipr/camera/camera.hpp"
#include <opencv2/core/core.hpp>

#include <zbar.h>

namespace kipr
{
  namespace camera
  {
    class HsvChannelImpl : public ChannelImpl
    {
    public:
      HsvChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ObjectVector findObjects(const config::Config &config);
      
    private:
      cv::Mat m_image;
    };
    
    class BarcodeChannelImpl : public ChannelImpl
    {
    public:
      BarcodeChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ObjectVector findObjects(const config::Config &config);

    private:
      cv::Mat m_gray;
      zbar::Image m_image;
      zbar::ImageScanner m_scanner;
    };

    class ArucoChannelImpl : public ChannelImpl
    {
    public:
      ArucoChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ObjectVector findObjects(const config::Config &config);

    private:
      cv::Mat m_image;
    };
  }
}

#endif

#endif

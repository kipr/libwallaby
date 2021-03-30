/*
 * channel_p.hpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#ifdef WITH_VISION_SUPPORT

#ifndef _CHANNEL_P_HPP_
#define _CHANNEL_P_HPP_

#include "wallaby/camera.hpp"
#include <opencv2/core/core.hpp>

#include <zbar.h>

namespace Private
{
  namespace Camera
  {
    class HsvChannelImpl : public ::Camera::ChannelImpl
    {
    public:
      HsvChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ::Camera::ObjectVector findObjects(const Config &config);
      
    private:
      cv::Mat m_image;
    };
    
    class BarcodeChannelImpl : public ::Camera::ChannelImpl
    {
    public:
      BarcodeChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ::Camera::ObjectVector findObjects(const Config &config);

    private:
      cv::Mat m_gray;
      zbar::Image m_image;
      zbar::ImageScanner m_scanner;
    };

    class ArucoChannelImpl : public ::Camera::ChannelImpl
    {
    public:
      ArucoChannelImpl();
      virtual void update(const cv::Mat &image);
      virtual ::Camera::ObjectVector findObjects(const Config &config);

    private:
      cv::Mat m_image;
    };
  }
}

#endif

#endif

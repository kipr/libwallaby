#ifndef _KIPR_CAMERA_CHANNEL_P_HPP_
#define _KIPR_CAMERA_CHANNEL_P_HPP_

#include "kipr/camera/camera.hpp"
#include "kipr/camera/channel_impl.hpp"
#include "kipr/camera/object.hpp"
#include <opencv2/core/core.hpp>

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
  }
}

#endif


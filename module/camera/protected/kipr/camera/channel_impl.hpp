#ifndef _KIPR_CAMERA_CHANNEL_IMPL_HPP_
#define _KIPR_CAMERA_CHANNEL_IMPL_HPP_

#include "kipr/camera/object.hpp"
#include "kipr/config/config.hpp"
#include "kipr/camera/channel.hpp"

#include <opencv2/core/core.hpp>

namespace kipr
{
  namespace camera
  {
    class Device;

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

    typedef std::vector<Channel *> ChannelPtrVector;
  }
}

#endif
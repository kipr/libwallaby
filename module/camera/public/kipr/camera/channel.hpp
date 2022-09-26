#ifndef _KIPR_CAMERA_CHANNEL_HPP_
#define _KIPR_CAMERA_CHANNEL_HPP_

#include "kipr/camera/object.hpp"
#include "kipr/config/config.hpp"

namespace kipr
{
  namespace camera
  {
    class Device;
    class ChannelImpl;

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
  }
}

#endif

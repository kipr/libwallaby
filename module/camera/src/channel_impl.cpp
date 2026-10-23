#include "kipr/camera/channel_impl.hpp"
#include "channel_p.hpp"

#include "logger.hpp"

using namespace kipr;
using namespace kipr::camera;

ChannelImpl::ChannelImpl() : m_dirty(true) {}

ChannelImpl::~ChannelImpl() {}

void ChannelImpl::setImage(const cv::Mat &image)
{
  if (image.empty())
  {
    m_image = cv::Mat();
    m_dirty = true;
    return;
  }
  m_image = image;
  m_dirty = true;
}

ObjectVector ChannelImpl::objects(const config::Config &config)
{
  if (m_dirty)
  {
    update(m_image);
    m_dirty = false;
  }
  return findObjects(config);
}

std::map<std::string, ChannelImpl *> ChannelImplManager::m_channelImpls = {
  {"hsv", new HsvChannelImpl()},
};

void ChannelImplManager::setImage(const cv::Mat &image)
{
  std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.begin();
  for (; it != m_channelImpls.end(); ++it)
    it->second->setImage(image);
}

ChannelImpl *ChannelImplManager::channelImpl(const std::string &name)
{
  std::map<std::string, ChannelImpl *>::iterator it = m_channelImpls.find(name);
  return (it == m_channelImpls.end()) ? 0 : it->second;
}
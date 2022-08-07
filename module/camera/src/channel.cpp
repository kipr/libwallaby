#include "kipr/camera/channel.hpp"
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
  {"qr", new BarcodeChannelImpl()},
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

// Channel //

Channel::Channel(Device *device, const config::Config &config)
    : m_device(device), m_config(config), m_impl(0), m_valid(false)
{
  m_objects.clear();
  const std::string type = config.stringValue("type");
  if (type.empty())
  {
    logger.error() << "No type specified in config.";
    return;
  }

  m_impl = ChannelImplManager::channelImpl(type);
  if (!m_impl)
  {
    logger.error() << "Type " << type << " not found";
    return;
  }
}

Channel::~Channel() {}

void Channel::invalidate() { m_valid = false; }

struct AreaComparator
{
public:
  bool operator()(const Object &left, const Object &right)
  {
    return left.boundingBox().area() > right.boundingBox().area();
  }
} LargestAreaFirst;

const ObjectVector *Channel::objects() const
{
  if (!m_impl)
    return 0;
  if (!m_valid)
  {
    m_objects.clear();
    m_objects = m_impl->objects(m_config);
    std::sort(m_objects.begin(), m_objects.end(), LargestAreaFirst);
    m_valid = true;
  }
  return &m_objects;
}

Device *Channel::device() const { return m_device; }

void Channel::setConfig(const config::Config &config)
{
  m_config = config;
  invalidate();
}
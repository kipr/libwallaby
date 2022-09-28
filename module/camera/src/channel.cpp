#include "kipr/camera/channel.hpp"
#include "channel_p.hpp"

#include "logger.hpp"

using namespace kipr;
using namespace kipr::camera;

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
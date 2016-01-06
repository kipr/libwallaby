/*
 * boyd_p.hpp
 *
 *  Created on: Jan 6, 2016
 *      Author: Nafis Zaman
 */

#ifndef _CAMERA_P_HPP
#define _CAMERA_P_HPP

#include <daylite/node.hpp>
#include <daylite/publisher.hpp>
#include <daylite/subscriber.hpp>
#include <daylite/bson.hpp>
#include <battlecreek/frame_data.hpp>

#include "wallaby/camera.hpp"

namespace Private
{
  class Camera
  {
  public:    
    bool open(const int deviceNumber, const int width, const int height);
    bool close();
    
    void loadConfig(const std::string &name);
    
    void setWidth(const int width);
    void setHeight(const int height);
    
    int width();
    int height();
    
    bool update();
    
    void setConfigBasePath(const std::string &path);
    
    bool checkChannel(const int channel);
    bool checkChannelObject(const int channel, const int object);
    
    const std::vector<uint8_t> *rawPixels();
    const std::vector<std::vector<Object>> *channelBlobs();
    
    static Camera *instance();
  
  private:
    Camera();
    
    bool isOpen();
    void receivedFrame(const daylite::bson &msg, void *arg);
    
    std::shared_ptr<daylite::node> m_node;
    std::shared_ptr<daylite::subscriber> m_frameSub;
    std::shared_ptr<daylite::publisher> m_settingsPub;
    
    daylite::bson m_latestFrameBson;
    boyd::frame_data m_userFrame;
    std::vector<std::vector<Object>> m_channelBlobs;
    
    // Are m_userFrame and m_channelBlobs valid?
    bool m_userFrameValid;
    
    // Has m_latestFrameBson changed since last update call?
    bool m_newFrameAvailable;
  };
}

#endif
//
//  UDPvideo.hpp
//
//
//  Created by gene on 9/1/20.
//

#ifndef UDPVideo_hpp
#define UDPVideo_hpp

#include <arpa/inet.h>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

class VideoFrameProcessor
{
  std::queue<AVPacket> m_frame_data_vector;
  std::mutex m_vid_mutex;
  std::mutex m_vid_frame_mutex;
  int m_ldestw, m_ldesth;

  AVCodecParserContext *m_parser;
  AVDictionary *m_av_dict_h264 = NULL;
  AVCodecContext *m_pcodec_ctx_h264;

  int m_sockfd;
  struct sockaddr_in m_cliaddr;
  struct sockaddr_in m_servaddr;

public:
  VideoFrameProcessor(const char *drone_ip_address,
                      const short unsigned int drone_port, const int destw,
                      const int desth);

  ~VideoFrameProcessor(void) throw();
  bool get_pframe_from_list(cv::OutputArray image); // AVFrame& newFrame);

private:
  void add_frame_to_list(AVPacket packet);

  void add_pframe_to_list(AVFrame *newFrame);

  void run_vfp();
  void run_vdr();
  std::thread *m_vfp_thread;
  std::thread *m_vdr_thread;

  bool m_vfp_running;
  bool m_vdr_running;
  bool m_udp_video_up;
  AVFrame *m_next_frame;
};

class UdpVideo : public cv::VideoCapture
{

public:
  UdpVideo(const char *drone_ip_address, const short unsigned int drone_port,
           const int destw, const int desth);
  ~UdpVideo();
  bool isOpened() const;
  // bool read(cv::Mat& Image);
  bool read(cv::OutputArray image) override;

private:
  bool m_udp_opened;
  VideoFrameProcessor *m_data_receiver;
};

#endif /* UDPVideo_hpp */

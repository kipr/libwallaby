//
//  USBNETvideo.hpp
//
//
//  Created by gene on 7/25/25.
//

#ifndef USBNETVideo_hpp
#define USBNETVideo_hpp

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
#include <opencv2/opencv.hpp>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

class UsbFrameProcessor
{
  std::queue<AVPacket> m_frame_data_vector;
  std::mutex m_vid_mutex;
  std::mutex m_vid_frame_mutex;
  int m_ldestw, m_ldesth;

  AVDictionary *m_av_dict_h264 = NULL;
  AVCodecContext *m_pcodec_ctx_h264;

  int m_sockfd;
  struct sockaddr_in m_cliaddr;
  struct sockaddr_in m_servaddr;

public:
  UsbFrameProcessor(const char *drone_ip_address,
                      const short unsigned int drone_port, const int destw,
                      const int desth);

  ~UsbFrameProcessor(void) throw();
  bool get_pframe_from_list(cv::OutputArray image, char* & object_list); // AVFrame& newFrame);
  cv::Mat m_next_frame;

  void add_pframe_to_list(cv::Mat &newFrame, char * object_list);

  char * m_next_object_list;

private:
  void run_vfp();
  void run_vdr();
  std::thread *m_vfp_thread;
  std::thread *m_vdr_thread;

  bool m_vfp_running;
  bool m_vdr_running;
  bool m_udp_video_up;
  bool m_new_frame = false;
};

class UsbnetVideo : public cv::VideoCapture
{

public:
  UsbnetVideo(const char *drone_ip_address, const short unsigned int drone_port,
           const int destw, const int desth);
  ~UsbnetVideo();
  bool isOpened() const;
  //bool read(cv::Mat* Image) override;
  bool read(cv::OutputArray image) override;
  static char * m_object_list;

private:
  bool m_udp_opened;
  UsbFrameProcessor *m_data_receiver;
};

char * ai_camera_command(char * command, int bufferlength);
bool ai_camera_check();

#endif /* USBNETVideo_hpp */

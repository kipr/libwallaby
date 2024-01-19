#include "UDPVideo.hpp"

#define DEST_PIXEL_FMT AV_PIX_FMT_BGR24
#define TELLO_H 720
#define TELLO_W 1280
#define UDP_RCV_TIMEOUT 500000 // in microseconds

#define FRAME_TIMEOUT 5000 // in milliseconds

//#define DEBUG_UDP 1
#include <ostream>
using namespace std;

int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame,
           AVPacket *pkt)
{
  int ret;

  *got_frame = 0;

  if (pkt)
  {
    ret = avcodec_send_packet(avctx, pkt);
    std::cout << "decode - avcodec_send_packet - ret: " << ret << std::endl;

    // In particular, we don't expect AVERROR(EAGAIN), because we read all
    // decoded frames with avcodec_receive_frame() until done.
    if (ret < 0)
      return ret == AVERROR_EOF ? 0 : ret;
  }

  ret = avcodec_receive_frame(avctx, frame);
  std::cout << "decode - avcodec_receive_frame - ret: " << ret << std::endl;

  if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    return ret;
  if (ret > 0)
    *got_frame = 1;

  return 0;
}

VideoFrameProcessor::VideoFrameProcessor(
    const char *drone_ip_address, const short unsigned int drone_port,
    const int destw, const int desth)
{
  // todo: allow caller to set ip address
  m_udp_video_up = true;
  m_vdr_running = false;
  m_next_frame = NULL;

  m_ldestw = destw;
  m_ldesth = desth;

  // Create a socket file descriptor

  if ((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    std::cout << "socket creation failed" << std::endl
              << std::flush;
    return;
  }

  memset(&m_servaddr, 0, sizeof(m_servaddr));

  // Filling server information
  m_servaddr.sin_family = AF_INET;
  m_servaddr.sin_port = htons(drone_port);
  m_servaddr.sin_addr.s_addr = INADDR_ANY;

  int on = 1;
  setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  struct timeval poll_timeout;
  poll_timeout.tv_sec = 0;
  poll_timeout.tv_usec = UDP_RCV_TIMEOUT;
  setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, &poll_timeout,
             sizeof(poll_timeout));

  // Bind the socket with the server address
  if (bind(m_sockfd, (const struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) <
      0)
  {
    std::cout << "bind failed: " << strerror(errno) << std::endl
              << std::flush;
    return;
  }

  m_vfp_running = false;

  // setup the codec and the parser
  avcodec_register_all(); // register all of the codecs

  // link in codecs
  AVCodec *p_codec_h264 = avcodec_find_decoder(AV_CODEC_ID_H264);

  // Hardware codec
  // AVCodec * p_codec_h264 = avcodec_find_decoder_by_name("h264_mmal");
  if (p_codec_h264 == NULL)
  {
    std::cout << "codec not found" << std::endl
              << std::flush;
    return;
  }

  m_pcodec_ctx_h264 = avcodec_alloc_context3(p_codec_h264);

  if (m_pcodec_ctx_h264 == NULL)
  {
    std::cout << "VideoFrameProcessor::run - could not allocate codec context"
              << std::endl
              << std::flush;
    return;
  }

  if (p_codec_h264->capabilities & AV_CODEC_CAP_TRUNCATED)
  {
    m_pcodec_ctx_h264->flags |= AV_CODEC_FLAG_TRUNCATED;
  }

  avcodec_open2(m_pcodec_ctx_h264, p_codec_h264, &m_av_dict_h264);

  // setup the parser

  m_parser = av_parser_init(AV_CODEC_ID_H264);

  // start the frame processing thread
  m_vfp_thread = new std::thread([this]()
                                 { run_vfp(); });

  while (!m_vfp_running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  // start up the UDP packet receiver thread
  m_vdr_thread = new std::thread([this]()
                                 { run_vdr(); });

  while (!m_vdr_running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  std::cout << "VideoFrameProcessor initialized" << std::endl;
}

VideoFrameProcessor::~VideoFrameProcessor() throw()
{
  std::cout << "VideoFrameProcessor - terminating" << std::endl;

  m_udp_video_up = false;
  try
  {
    m_vdr_thread->join();
    m_vfp_thread->join();
  }

  catch (int ex)
  {
    std::cout << "~VideoFrameProcessor - exception :" << ex
              << std::endl
              << std::flush;
  }
  close(m_sockfd);

  // Cleanup
  av_parser_close(m_parser);
  std::cout << "VideoFrameProcessor - terminated" << std::endl;
}

void VideoFrameProcessor::run_vdr()
{
  unsigned int packet_count;
  unsigned int pos;

  std::cout << "VideoFrameProcessor started" << std::endl;
  unsigned int payload_size = 65536;
  size_t receive_size;
  bool first_keyframe = false;

  AVPacket packet;

  av_init_packet(&packet);

  int in_len;
  unsigned char *in_data;
  int len = 0;

  // Video packet processing loop
  // receive UDP packets, reassemble frame packets and send them on
  m_vdr_running = true;

  while (m_udp_video_up)
  {
    unsigned char packetdata[65536];
    socklen_t soc_len = sizeof(m_cliaddr); // len is value/resuslt
    receive_size = recvfrom(m_sockfd, (char *)packetdata, payload_size, 0,
                            (struct sockaddr *)&m_cliaddr, &soc_len);
    if ((int)receive_size < 0)
    {
      std::cout << "run_vdr: " << strerror(errno) << std::endl;
      continue;
    }

    in_len = receive_size;
    in_data = packetdata;

    // rebuild frames from the UDP packets that are arriving
    while (in_len)
    {
      len = av_parser_parse2(m_parser, m_pcodec_ctx_h264, &(packet.data),
                             &(packet.size), in_data, in_len, 0, 0,
                             AV_NOPTS_VALUE);
      in_data += len;
      in_len -= len;

      // have a packet, prepare it for processing
      if (packet.size != 0)
      {
        // annotate key frames
        if (m_parser->key_frame == 1 ||
            (m_parser->key_frame == -1 &&
             m_parser->pict_type == AV_PICTURE_TYPE_I))
          packet.flags |= AV_PKT_FLAG_KEY;

        if ((m_parser->key_frame == -1) &&
            (m_parser->pict_type == 0) &&
            (packet.flags & AV_PKT_FLAG_KEY))
          packet.flags |= AV_PKT_FLAG_KEY;

        if (m_parser->key_frame)
          first_keyframe = true;

        // can only process after we receive a keyframe
        if (first_keyframe)
        {
          unsigned char *new_data;

          new_data = (unsigned char *)av_malloc(packet.size);
          memcpy(new_data, packet.data, packet.size);
          packet.data = new_data;
          add_frame_to_list(packet);
        }
        av_init_packet(&packet);
        packet.data = NULL;
      }
    }
  }
VDR_done:
  std::cout << "VideoFrameProcessor stopped" << std::endl;
}

#define FRAME_SLEEP_TIME 10 // in milliseconds
bool VideoFrameProcessor::get_pframe_from_list(cv::OutputArray image)
{
  unsigned long wait_time = 0;
  while (m_next_frame == NULL)
  {
    // wait until we get something in the vector
    std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_SLEEP_TIME));
    if (!m_udp_video_up)
      return false;

    wait_time += FRAME_SLEEP_TIME;
    if (wait_time > FRAME_TIMEOUT)
    {
      std::cout << "Frame timeout" << std::endl
                << std::flush;
      return false;
    }
  }
  m_vid_frame_mutex.lock();
  AVFrame dst;
  cv::Mat m;

  image.create(m_next_frame->height, m_next_frame->width, CV_8UC3);
#if DEBUG_UDP
  std::cout << "VideoFrameProcessor::get_pframe_from_list h: " << m_next_frame->height
            << " w: " << m_next_frame->width
            << std::hex
            << " dataptr: " << (void *)m_next_frame->data[0]
            << std::dec
            << " linesize[0] " << m_next_frame->linesize[0]
            << " linesize[1] " << m_next_frame->linesize[1]
            << " linesize[2] " << m_next_frame->linesize[2]
            << std::endl
            << std::flush;
#endif
  cv::Mat image2 = image.getMat();

  memset(&dst, 0, sizeof(dst));

  dst.data[0] = (uint8_t *)image2.data;
  avpicture_fill((AVPicture *)&dst, dst.data[0], AV_PIX_FMT_BGR24,
                 m_next_frame->width, m_next_frame->height);

  struct SwsContext *convert_ctx = NULL;
  enum AVPixelFormat src_pixfmt =
      (AVPixelFormat)m_next_frame->format; // AV_PIX_FMT_BGR24;
  enum AVPixelFormat dst_pixfmt = AV_PIX_FMT_BGR24;
  convert_ctx = sws_getContext(m_next_frame->width, m_next_frame->height,
                               src_pixfmt, m_ldestw, m_ldesth, dst_pixfmt,
                               SWS_BILINEAR, NULL, NULL, NULL);

  sws_scale(convert_ctx, m_next_frame->data, m_next_frame->linesize, 0,
            m_ldesth, dst.data, dst.linesize);
  sws_freeContext(convert_ctx);

  av_free(m_next_frame);
  m_next_frame = NULL;

  m_vid_frame_mutex.unlock();
  return true;
}

void VideoFrameProcessor::add_pframe_to_list(AVFrame *newFrame)
{
  m_vid_frame_mutex.lock();

  // make sure the requestor receives the most recent frame
  // so we have only a one frame queue
  if (m_next_frame != NULL)
  {
    av_free(m_next_frame);
  }
  m_next_frame = newFrame;
#if 0
	std::cout << "VideoFrameProcessor::add_pframe_to_list h: " << newFrame->height
    << " w: " << newFrame->width
    << std::hex
    << " dataptr: " << (void *) newFrame->data[0]
    << std::dec
    << std::endl << std::flush;
#endif
  newFrame = NULL;
  m_vid_frame_mutex.unlock();
}

void VideoFrameProcessor::add_frame_to_list(AVPacket frameData)
{
  m_vid_mutex.lock();

  // When a new Key Frame arrives, flush the frame queue.
  // This way the most recent frame is sent to the codac
  if ((frameData.flags & AV_PKT_FLAG_KEY) == AV_PKT_FLAG_KEY)
  {
    std::queue<AVPacket> empty;
    std::swap(m_frame_data_vector, empty);
  }
  m_frame_data_vector.push(frameData);
  m_vid_mutex.unlock();
#if 0
	std::cout << "::add_frame_to_list --> Packet added to list frameData.data: "
        << std::hex
        << (void *) frameData.data
        << " frameData.size " << frameData.size
        << std::dec
        << std::endl << std::flush;
#endif
}

void VideoFrameProcessor::run_vfp()
{
  int FrameFinished;
  AVPacket packet;
  struct SwsContext *img_convert_ctx = NULL;

  // setup the datastructures for the codes (move this stuff somewhere else...

  std::cout << "VideoFrameProcessor::run - Starting" << std::endl;

  m_vfp_running = true;

  while (1)
  {
    m_vid_mutex.lock();

    while ((m_frame_data_vector.empty()) /* ||waitingForReceiver*/)
    {
      // wait until we get something in the vector
      m_vid_mutex.unlock(); // release the lock before we sleep
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      if (m_udp_video_up == false)
      {
        cout << "run_vfp - time to get out" << std::endl;
        goto VFP_done;
      }
      m_vid_mutex.lock(); // grab the lock so we can safely test
    }

    {
      packet = m_frame_data_vector.front();
      m_frame_data_vector.front().data = NULL;
      m_frame_data_vector.pop();
    }
    m_vid_mutex.unlock();

    // got a frame, now decode it...
    {

      AVFrame *pFrame = av_frame_alloc();
#ifdef DEBUG_UDP
      std::cout << "VideoFrameProcessor::run_vfp new packet data: "
                << std::hex
                << (void *)packet.data
                << std::dec
                << " size: " << packet.size
                << std::endl
                << std::flush;
#endif
      int avresult = avcodec_decode_video2(m_pcodec_ctx_h264, pFrame,
                                           &FrameFinished, &packet);
      av_free(packet.data);

      // int avresult = decode(pCodecCtxH264, pFrame, &FrameFinished, &packet);
#ifdef DEBUG_UDP
      std::cout << "VideoFrameProcessor::run_vfp-->Decoding complete "
                << "FrameFinished: " << FrameFinished
                << " avresult: " << avresult
                << std::endl
                << std::flush;
#endif
      if (FrameFinished == 0) //  no output this time arounf
      {
        std::cout << "-->frame not decoded\n";
        continue;
      }
      else // convert the image into something the KIPR SW can understand
      {
#ifdef DEBUG_UDP
        cout << "VideoFrameProcessor::run_vfp data: " << std::hex
             << (void *)pFrame->data[0]
             << std::dec
             << " pFrame->linesize[0]: " << pFrame->linesize[0]
             << " linesize[1]: " << pFrame->linesize[1]
             << " width: " << pFrame->width
             << " height: " << pFrame->height
             << " key_frame: " << pFrame->key_frame
             << std::endl
             << std::flush;
#endif
        if (pFrame->data[0] == NULL)
        {
          std::cout << "VideoFrameProcessor_VFP::run ***Warning*** pointer to frame "
                       "is NULL"
                    << std::endl
                    << std::flush;
          continue;
        }

        // pass it on to the consumer
        add_pframe_to_list(pFrame);
      }
    }
    //	std::this_thread::sleep_for (std::chrono::milliseconds(20));
  }
VFP_done:
  std::cout << "run_vfp terminating" << std::endl;
}

UdpVideo::UdpVideo(const char *drone_ip_address,
                   const short unsigned int drone_port, const int destw,
                   const int desth)
{
  m_udp_opened = true;
  m_data_receiver =
      new VideoFrameProcessor(drone_ip_address, drone_port, destw, desth);
  std::cout << "UDP Video started" << std::endl;
}

UdpVideo::~UdpVideo()
{
  m_udp_opened = false;
  delete m_data_receiver;
  std::cout << "~UdpVideo done\n";
}

bool UdpVideo::isOpened() const { return m_udp_opened; }

bool UdpVideo::read(cv::OutputArray image)
{
  bool retval = false;
  if (isOpened())
    retval = m_data_receiver->get_pframe_from_list(image);
  return retval;
}

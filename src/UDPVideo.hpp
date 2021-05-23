//
//  UDPVideo.hpp
//  
//
//  Created by gene on 9/1/20.
//

#ifndef UDPVideo_hpp
#define UDPVideo_hpp

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <queue>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}

    struct Image
    {
      unsigned char * data; //1280x720x3 MAX out
      int width;
      int height;
    };
  
 struct FrameData
 {
   unsigned char * data;
   unsigned int size;
 };

   class VideoFrameProcessor 
  {
   static Image videoBuffer[2]; // Mod by Jeremy
   static long videoTimestamp[2]; // Mod by Jeremy
    bool enableCbcuiVision; // Mod by Jeremy
    static std::queue<AVPacket> FrameDataVector; 
    static std::queue<AVFrame&> FrameOutput;
    static std::mutex VidMutex;
    static std::mutex VidFrameMutex;
    static bool waitingForReceiver;
    static int ldestw, ldesth;

	static AVCodecParserContext  *parser;
	AVDictionary *AVDictH264 = NULL;
        static AVCodecContext * pCodecCtxH264;

	static int sockfd;
   static struct sockaddr_in cliaddr;
   struct sockaddr_in servaddr;

    std::string myDroneAddress;
    short unsigned int myDronePort;


    public:

    VideoFrameProcessor(const char* DroneIpAddress, const short unsigned int DronePort, const int destw, const int desth);
  
    //VideoFrameProcessor(const int destw, const int desth);
    ~VideoFrameProcessor(void) throw();
    
    static void addFrametoList(AVPacket packet);
    void receiverDone(void);

    static bool getpFramefromList (cv::OutputArray image); //AVFrame& newFrame);
    static void addpFrametoList(AVFrame* newFrame);

     static void run_VFP();	
     static void run_VDR();

   private:
	std::thread* VFP_thread;
    std::thread* VDR_thread;

	static bool VFPrunning;
        static bool VDRrunning;
	    static bool UDPVideoUP;
  };

#if 0  
  class VideoDataReceiver
  {
    std::mutex myMutex;
    std::string myDroneAddress;
    short unsigned int myDronePort;
    static VideoFrameProcessor* myVideoFrameProcessor;
   // unsigned char myVideoData[921600]; //640x480x3 MAX out
    unsigned int videoDataLength;

   static int sockfd;
   static struct sockaddr_in cliaddr;
   struct sockaddr_in servaddr;
   int ldestw, ldesth;

   struct timespec sleep;
   struct timespec sleep_left;

  public:
    VideoDataReceiver(const char* DroneIpAddress, const short unsigned int DronePort, const int destw, const int desth);
    ~VideoDataReceiver() throw ();

    bool getpFramefromList (cv::OutputArray); //AVFrame& newFrame);
    
    static void run_VDR();
  private:
    std::thread* VDR_thread;
  protected:
    static bool UDPVideoUP;
    static bool VDRrunning;
  };
#endif

  class UDPVideo: public cv::VideoCapture
  {
     VideoFrameProcessor* DataReceiver;

     public:
        UDPVideo(const char* DroneIpAddress, const short unsigned int DronePort, const int destw, const int desth);
        ~UDPVideo();
	bool isOpened() const;
	//bool read(cv::Mat& Image);
        bool read(cv::OutputArray image) override;

     protected:
	bool UDPOpened;
	cv::Mat tmp_image;
  };




#endif /* UDPVideo_hpp */

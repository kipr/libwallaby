#include "UDPVideo.hpp"

#define DEST_PIXEL_FMT AV_PIX_FMT_BGR24
#define TELLO_H 720
#define TELLO_W 1280

// keep the linker happy with statics
int VideoFrameProcessor::safeBufferIndex;
int VideoFrameProcessor::ldestw;

std::queue<FrameData> VideoFrameProcessor::FrameDataVector;
//std::queue<AVFrame&> VideoFrameProcessor::FrameOutput;

int VideoFrameProcessor::ldesth;
int VideoDataReceiver::sockfd;
std::mutex VideoFrameProcessor::VidMutex;
std::mutex VideoFrameProcessor::VidFrameMutex;
bool VideoFrameProcessor::waitingForReceiver;
struct sockaddr_in VideoDataReceiver::cliaddr;
long VideoFrameProcessor::videoTimestamp[2];
Image VideoFrameProcessor::videoBuffer[2];
VideoFrameProcessor* VideoDataReceiver::myVideoFrameProcessor;
bool VideoDataReceiver::UDPVideoUP;
bool VideoDataReceiver::VDRrunning;
bool VideoFrameProcessor::VFRrunning;
AVCodecParserContext* VideoFrameProcessor::parser;


static AVFrame* NextFrame;

#if 0
int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
    int ret;

    *got_frame = 0;

    if (pkt) {
        ret = avcodec_send_packet(avctx, pkt);
	::printf("decode - avcodec_send_packet - ret: %d\n", ret);
        // In particular, we don't expect AVERROR(EAGAIN), because we read all
        // decoded frames with avcodec_receive_frame() until done.
        if (ret < 0)
            return ret == AVERROR_EOF ? 0 : ret;
    }

    ret = avcodec_receive_frame(avctx, frame);
	        ::printf("decode - avcodec_receive_frame - ret: %d\n", ret);

    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
        return ret;
    if (ret >= 0)
        *got_frame = 1;

    return 0;
}
#endif

VideoDataReceiver::VideoDataReceiver(const char* DroneIpAddress,
					const short unsigned int DronePort,
					const int destw,
					const int desth)
  {
    myDroneAddress = DroneIpAddress;
    myDronePort = DronePort;
    UDPVideoUP = true;
    VDRrunning = false;

    ldestw = destw;
    ldesth = desth;

    // Create a socket file descriptor

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed");fflush(NULL); 
        exit(EXIT_FAILURE); 
    }

    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(myDronePort); 
    servaddr.sin_addr.s_addr = INADDR_ANY;       
    
    int on = 1;
    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

// Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed");fflush(NULL); 
        exit(EXIT_FAILURE); 
    }

    // start the VideFrameProcessor

	::printf("VideoDataReceiver starting VideoFrameProcessor ldestw: %d ldesth %d\n",
				ldestw,
				ldesth);

      myVideoFrameProcessor = new VideoFrameProcessor(ldestw, ldesth);
      VDR_thread = new std::thread([this] () {run();} );

	while(!VDRrunning)
	{
		struct timespec sleep;
        	struct timespec sleep_left;

        	sleep.tv_sec = 0;
        	sleep.tv_nsec = 20000;
        	nanosleep(&sleep, &sleep_left);
	}

        ::printf("VideoDataReceiver initialized\n");fflush(NULL);

  }

  VideoDataReceiver::~VideoDataReceiver() throw ()
  {
      printf ("VideoDataReceiver - terminating\n");fflush(NULL);
      close (sockfd);
      UDPVideoUP = false;
      try
      {
        VDR_thread->join();
      }
      catch (int ex)
      {
        std::cout << "~VideoDataReceiver - exception :" << ex << std::endl;fflush(NULL);
      }
  }
  
struct Packet
	{
	unsigned char data[1460];
	unsigned int size;
	struct Packet* next;
	};

  void VideoDataReceiver::run()
  {
      unsigned char * myVideoData;
      FrameData frameData;
      unsigned int packet_count;
      unsigned int pos;

	AVPacket * AVpkt;

      struct Packet* packet;
	struct Packet* first_packet;
	struct Packet* last_packet;
	struct Packet* curr_packet;
      
    std::cout << "VideoDataReceiver started\n";
      unsigned int payload_size;
      size_t receive_size;
    unsigned int ex_payload_size;
      bool HeaderSearchMode = false;       // for a lost header
      unsigned int next_pos;

     VDRrunning = true;
      while(UDPVideoUP)
      {
        payload_size = 65536;  // Tello UDP packet size
	receive_size = 0;      // initialize
        next_pos = 0;
        ex_payload_size = payload_size;
	packet_count = 0;
	first_packet = NULL;
	do
	{
	  	packet = new Packet; 
		if(first_packet == NULL)
		{
			first_packet = packet;
		}
		else
		{
			last_packet->next = packet;
		}

		packet->next = NULL;

          	socklen_t  len = sizeof(cliaddr);  //len is value/resuslt
////UDP receive  frame
          	receive_size = recvfrom(sockfd, (char *)packet->data, payload_size,
                           MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                           &len);
		packet->size = receive_size;
		packet_count++;
		//::printf("VideoDataReceiver::run - receive_size %d packet->size %d data %x\n", receive_size, packet->size, packet->data);
		last_packet = packet;
	} while(!(receive_size < 1460));
//////

          myVideoData = (unsigned char *) malloc (1460 * (packet_count));

	  curr_packet = first_packet;
	  pos = 0;

	 frameData.size = 0;

	  while(curr_packet != NULL)
	  {
		//::printf("VideoDataReceive::run compile curr_packet->size %d data %x pos %d\n", curr_packet->size, curr_packet->data, pos);
		memcpy(&myVideoData[pos], curr_packet->data, curr_packet->size);	
		pos += curr_packet->size;	
                frameData.size += curr_packet->size;
		last_packet = curr_packet;
		curr_packet = curr_packet->next;
		free(last_packet);
	  }

	  frameData.data = myVideoData;

	::printf("VideoDataReceiver::run - packet ready frameData %p frameData.size %d\n", (void *)frameData.data, frameData.size);
         
           myVideoFrameProcessor->addFrametoList(frameData);
    }
        

    std::cout << "VideoDataReceiver stopped\n";
}

bool VideoFrameProcessor::getpFramefromList (cv::OutputArray image) //(AVFrame& newFrame)
{
	while(NextFrame == NULL)
       {    
           // wait until we get something in the vector
           std::this_thread::sleep_for (std::chrono::milliseconds(10));
	   //return false;
       }
	VidFrameMutex.lock();
	AVFrame dst;
        cv::Mat m;

	image.create(NextFrame->height,
                     NextFrame->width,
                     CV_8UC3);

       ::printf("VideoFrameProcessor::getpFramefromList: %d w: %d dataptr %x linesize[0] %d linesize[1] %d linesize[2] %d\n",
                        NextFrame->height,
                        NextFrame->width,
                        NextFrame->data[0],
                        NextFrame->linesize[0],
                        NextFrame->linesize[1],
                        NextFrame->linesize[2]);fflush(NULL);

       cv::Mat image2 = image.getMat();
       ::printf("VideoFrameProcessor::getpFramefromList image2.data %x\n", image2.data);fflush(NULL);

        memset(&dst, 0, sizeof(dst));

        dst.data[0] = (uint8_t *)image2.data;
        avpicture_fill ((AVPicture *)&dst,
			dst.data[0],
			AV_PIX_FMT_BGR24,
			NextFrame->width,
			NextFrame->height);

        struct SwsContext *convert_ctx=NULL;
        enum AVPixelFormat src_pixfmt = (AVPixelFormat) NextFrame->format; //AV_PIX_FMT_BGR24;
        enum AVPixelFormat dst_pixfmt = AV_PIX_FMT_BGR24;
        convert_ctx = sws_getContext(NextFrame->width,
					NextFrame->height,
					src_pixfmt,
					ldestw,
					ldesth,
					dst_pixfmt,
                        		SWS_BILINEAR, NULL, NULL, NULL);

        sws_scale(convert_ctx, NextFrame->data, NextFrame->linesize, 0,ldesth,
                        dst.data, dst.linesize);
        sws_freeContext(convert_ctx);

	av_free(NextFrame);
	NextFrame = NULL;

	VidFrameMutex.unlock();
	return true;
}

bool VideoDataReceiver::getpFramefromList (cv::OutputArray newFrame) // AVFrame& newFrame)
{
	bool retval;
	retval = myVideoFrameProcessor->getpFramefromList(newFrame);
#if 0
               ::printf( "VideoDataReceiver::getpFramefromList h: %d  w: %d dataptr %x\n ",
                            newFrame.height, newFrame.width, newFrame.data[0]); fflush(NULL);
#endif
	return retval;

}

void VideoFrameProcessor::addpFrametoList(AVFrame* newFrame)
{

	VidFrameMutex.lock();
		//std::queue<AVFrame*> empty;
		//std::swap(FrameOutput, empty);
		//FrameOutput.push(newFrame);
		if(NextFrame != NULL)
		{
			av_free(NextFrame);
		}
		NextFrame = newFrame;
		::printf( "VideoFrameProcessor::addpFrametoList h: %d  w: %d dataptr %x\n ",
                                 newFrame->height, newFrame->width, newFrame->data[0]);fflush(NULL);
		newFrame = NULL;
	VidFrameMutex.unlock();
}

  void  VideoFrameProcessor::addFrametoList(struct FrameData frameData)
  {
     VidMutex.lock();

	// The tello camera puts out frames of sizes 13 and 8
	// Just before it sends a key frame
	// So, we flush the queue and let the keyframe be next
#if 1
	if(frameData.size == 13)
	{
		//:: printf("addFrametoList - small packet received, flushing queue %d\n", frameData.size);
		//while(!FrameDataVector.empty())
		//{
	//		FrameDataVector.pop();
	//	}
		std::queue<FrameData> empty;
		std::swap(FrameDataVector, empty);
	
	}
 #endif 
     FrameDataVector.push(frameData);
     VidMutex.unlock(); 
     //::printf("-->Frame added to list\n");fflush(NULL);
  }

  //
  // VideoFrameProcessor  (EDM)
  //
  
  VideoFrameProcessor::VideoFrameProcessor(const int destw, const int desth)
  {
        enableCbcuiVision = false;
	ldesth = desth;
	ldestw = destw;
	
	VFRrunning = false;
	VFP_thread = new std::thread ([this]() {run();} );

	while(!VFRrunning)
	{
		struct timespec sleep;
        	struct timespec sleep_left;

        	sleep.tv_sec = 0;
        	sleep.tv_nsec = 20000;
        	nanosleep(&sleep, &sleep_left);
	}
	::printf("VideoFrameProcessor initialized\n"); fflush(NULL);
  }
  ///
  VideoFrameProcessor::~VideoFrameProcessor(void) throw()
  {
	av_parser_close(parser);

	printf("VideoFrameProcessor - terminated\n");fflush(NULL);
  }
  
  void VideoFrameProcessor::receiverDone()
  {
    waitingForReceiver = false;
  }
  
  void VideoFrameProcessor::run()
  {
    int FrameFinished;
    static AVPacket packet;
    static struct SwsContext * img_convert_ctx = NULL;
    bool waitForIFrame = true;      // must get an i-frame before anything else
    unsigned int FrameUsed;         // frame use out of the current list

    unsigned char *myVideoData;
    unsigned int VideoSize;
    AVDictionary *AVDictH264 = NULL;
    
    safeBufferIndex = 1;
    videoBuffer[0].height = videoBuffer[0].width = videoBuffer[1].height = videoBuffer[1].width = 0;
    videoTimestamp[0] = videoTimestamp[1] = 0;
    videoBuffer[0].data=videoBuffer[1].data = NULL;
    
    // setup the datastructures for the codes (move this stuff somewhere else...
    
    ::printf("VideoFrameProcessor::run - Starting\n"); fflush(NULL);

      avcodec_register_all(); // register all of the codecs
    //::printf("VideoFrameProcessor -- 1\n"); fflush(NULL);

      // link in codecs
      AVCodec * pCodecH264 = avcodec_find_decoder( AV_CODEC_ID_H264);

     //AVCodec * pCodecH264 = avcodec_find_decoder_by_name("h264_mmal");
	if (pCodecH264 == NULL)
	{
		printf("codec not found\n");fflush(NULL);
		exit(1);
	}

	AVCodecContext * pCodecCtxH264 = avcodec_alloc_context3(pCodecH264);

	if(pCodecCtxH264 == NULL)
	{
        	::printf("VideoFrameProcessor::run - could not allocate codec context\n");fflush(NULL);
		exit(1);
   	 }
      //pCodecCtxH264->pix_fmt = AV_PIX_FMT_YUV420P;
      //pCodecCtxH264->skip_frame = AVDISCARD_DEFAULT;
      //pCodecCtxH264->error_concealment = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
      //pCodecCtxH264->skip_loop_filter = AVDISCARD_DEFAULT;
      //pCodecCtxH264->workaround_bugs = FF_BUG_AUTODETECT;
      //pCodecCtxH264->codec_type = AVMEDIA_TYPE_VIDEO;
      //pCodecCtxH264->codec_id = AV_CODEC_ID_H264;
      //pCodecCtxH264->skip_idct = AVDISCARD_DEFAULT;
      //pCodecCtxH264->width = TELLO_W;
      //pCodecCtxH264->height = TELLO_H;

	if( pCodecH264->capabilities & AV_CODEC_CAP_TRUNCATED)
	{
    		pCodecCtxH264->flags |= AV_CODEC_FLAG_TRUNCATED;
  	}

	avcodec_open2(pCodecCtxH264, pCodecH264, &AVDictH264);

	// setup the parser

        parser = av_parser_init(AV_CODEC_ID_H264);

        waitingForReceiver = true;
	VFRrunning = true;
      
    while(1)
    {
	//AVFrame pFrameOutput;
      while((FrameDataVector.empty())/* ||waitingForReceiver*/)
      {
	   // wait until we get something in the vector
           std::this_thread::sleep_for (std::chrono::milliseconds(10));
      }
   
      VidMutex.lock();
      {
	FrameData frameData;
        unsigned int FrameCount = FrameDataVector.size();        // get the number of frames in the list
        
        unsigned int oldestFrame;

	frameData = FrameDataVector.front();
     
        myVideoData = frameData.data;
	VideoSize = frameData.size;
	FrameDataVector.pop();
      }
      VidMutex.unlock();
      // got a frame, now decode it...
          {
		uint8_t* out_buffer;
		unsigned int RGB_size;

		av_init_packet(&packet);
		::printf("VideoFrameProcessor::run *** parsing packet data %x size %d\n",
					myVideoData,
					VideoSize);
		int in_len = VideoSize;
		unsigned char * in_data = myVideoData;
		int len = 0;
		
		while(in_len)
		{
			len = av_parser_parse2(parser,
						pCodecCtxH264,
						&packet.data,
						&packet.size,
    						in_data,
						in_len,
    						0,
						0,
						AV_NOPTS_VALUE);
#if 0
		::printf("VideoFrameProcessor::run - parse loop in_len %d, in_data %x, len %d packet.size %d\n",
					in_len, in_data, len, packet.size);
#endif
			in_data += len;
			in_len  -= len;
			
			if(packet.size != 0)
			{
				break;
			}
		}

		::printf("VideoFrameProcessor::run - parser complete keyframe: %d\n", parser->key_frame);

		if((len == 0) && (packet.size == 0)
		{
			::printf("VideoFrameProcessor::run - parser done - need more data\n");
		}


		AVFrame* pFrame = av_frame_alloc();

		::printf("VideoFrameProcessor::run parsing done packet data %x size %d\n",
				packet.data,
				packet.size);fflush(NULL);
		if(packet.size == 0)
		{
			::printf("VideoFrameProcessor::run - no packet found by parser\n");
			continue;
		}

		int avresult =	avcodec_decode_video2(pCodecCtxH264, pFrame, &FrameFinished, &packet);

                ::printf("VideoFrameProcessor::run-->Decoding complete FrameFinished: %d avresult: %d\n", FrameFinished, avresult);fflush(NULL);

                if(FrameFinished == 0)    //  no output this time arounf
                {
                	waitForIFrame = true;
             		::printf("-->frame not decoded\n");fflush(NULL);
                	continue;
            	}	
           	 else                     // convert the image into something the KIPR SW can understand
            	{
		::printf("VideoFrameProcessor::run data %x pFrame->linesize[0] %d linesize[1] %d width %d height %d key_frame %d\n",
				pFrame->data[0],
				pFrame->linesize[0],
				pFrame->linesize[1],
				pFrame->width,
				pFrame->height,
				pFrame->key_frame);fflush(NULL);
		
		if(pFrame->data[0] == NULL)
		{
			::printf("VideoFrameProcessor::run ***Warning*** pointer to frame is NULL\n");fflush(NULL);
			continue;
		}
		addpFrametoList(pFrame);
            }
      }

      //std::thread::sleep(20);

	struct timespec sleep;
        struct timespec sleep_left;
        
	sleep.tv_sec = 0;
        sleep.tv_nsec = 20000;
        nanosleep(&sleep, &sleep_left);

      //::printf("vd length--> %d\n", videoDataLength);
      //::Thread::sleep(50);     // let the video buffer fill up so we can get the latest possible
    }
    //
  }


        UDPVideo::UDPVideo(const char* DroneIpAddress, const short unsigned int DronePort, const int destw, const int desth)
	{
		    UDPOpened = false;
		    NextFrame = NULL;
		    DataReceiver = new VideoDataReceiver(DroneIpAddress, DronePort, destw, desth);
		    //std::thread DR(VideoDataReceiver::run);
//			cv::namedWindow("tello");
		    UDPOpened = true;
			::printf("UDP Video started\n");fflush(NULL);
	}

	UDPVideo::~UDPVideo()
	{
		free(DataReceiver);
	}

	bool UDPVideo::isOpened() const
	{
		return UDPOpened;
  	}

	bool UDPVideo::read(cv::OutputArray image)
	{
		bool retval = true;
#if 0
		long timestamp;
		bool retval = true;
		AVFrame rImage;
		AVFrame& resultImage = rImage;
		struct timespec sleep;
        	struct timespec sleep_left;
		bool debug_sent = false;
		cv::Mat tmp_image;

                ::printf("UDPVideo::read  - waiting for image\n");
		//resultImage = av_frame_alloc();
		retval = DataReceiver->getpFramefromList(resultImage);
		::printf("UDPVideo::read - processing frame\n");

		if(!retval)
			return false;
		image.create(resultImage.height,
			 	resultImage.width,
				CV_8UC3);
		 ::printf("UDPVideo::read h: %d w: %d dataptr %x linesize[0] %d linesize[1] %d linesize[2] %d\n",
                                resultImage.height,
                                resultImage.width,
                                resultImage.data[0],
                                resultImage.linesize[0],
                                resultImage.linesize[1],
                                resultImage.linesize[2]);fflush(NULL);
		cv::Mat image2 = image.getMat();
		 ::printf("UDPVideo::read image2.data %x\n", image2.data);fflush(NULL);
		
//		image2.rows = resultImage.height;
//		image2.cols = resultImage.width;
	//	image2.data = resultImage->data[0];

		memcpy(image2.data, resultImage.data[0], resultImage.height * resultImage.width * 3);

		image2 = cv::Mat(resultImage.height,
				resultImage.width,
				CV_8UC3,
				resultImage.data[0],
				resultImage.linesize[0]);

 ::printf("UDPVideo::read - memcpy image2.data %x resultImage->data %x size %d\n",
                                image2.data, resultImage.data[0], resultImage.height * resultImage.width * 3);fflush(NULL);
#endif
                retval = DataReceiver->getpFramefromList(image);

		return retval;
     } 

#include "UDPVideo.hpp"

#define DEST_PIXEL_FMT AV_PIX_FMT_BGR24
#define TELLO_H 720
#define TELLO_W 1280

// keep the linker happy with statics
int VideoFrameProcessor::ldestw;

std::queue<AVPacket> VideoFrameProcessor::FrameDataVector;
//std::queue<AVFrame&> VideoFrameProcessor::FrameOutput;

int VideoFrameProcessor::ldesth;
int VideoFrameProcessor::sockfd;
std::mutex VideoFrameProcessor::VidMutex;
std::mutex VideoFrameProcessor::VidFrameMutex;

AVCodecContext *  VideoFrameProcessor::pCodecCtxH264;

bool VideoFrameProcessor::waitingForReceiver;
struct sockaddr_in VideoFrameProcessor::cliaddr;
bool VideoFrameProcessor::UDPVideoUP;
bool VideoFrameProcessor::VDRrunning;
bool VideoFrameProcessor::VFPrunning;
AVCodecParserContext* VideoFrameProcessor::parser;


static AVFrame* NextFrame;

#if 1
int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
	int ret;
	
 	*got_frame = 0;

	if (pkt)
	{
		ret = avcodec_send_packet(avctx, pkt);
		::printf("decode - avcodec_send_packet - ret: %d\n", ret);fflush(NULL);
        	// In particular, we don't expect AVERROR(EAGAIN), because we read all
        	// decoded frames with avcodec_receive_frame() until done.
        	if (ret < 0)
            	return ret == AVERROR_EOF ? 0 : ret;
    	}

    	ret = avcodec_receive_frame(avctx, frame);
        ::printf("decode - avcodec_receive_frame - ret: %d\n", ret);fflush(NULL);

	if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
		return ret;
	if (ret > 0)
		*got_frame = 1;

	return 0;
}
#endif

VideoFrameProcessor::VideoFrameProcessor(const char* DroneIpAddress,
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

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{ 
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

        VFPrunning = false;

	// setup the codec and the parser
    	avcodec_register_all(); // register all of the codecs

     	// link in codecs
      	AVCodec * pCodecH264 = avcodec_find_decoder( AV_CODEC_ID_H264);

	// Hardware codec
     	//AVCodec * pCodecH264 = avcodec_find_decoder_by_name("h264_mmal");
        if (pCodecH264 == NULL)
        {
                printf("codec not found\n");fflush(NULL);
                exit(1);
        }

        pCodecCtxH264 = avcodec_alloc_context3(pCodecH264);

        if(pCodecCtxH264 == NULL)
        {
                ::printf("VideoFrameProcessor::run - could not allocate codec context\n");fflush(NULL);
                exit(1);
        }

        if( pCodecH264->capabilities & AV_CODEC_CAP_TRUNCATED)
        {
                pCodecCtxH264->flags |= AV_CODEC_FLAG_TRUNCATED;
        }

        avcodec_open2(pCodecCtxH264, pCodecH264, &AVDictH264);

        // setup the parser

        parser = av_parser_init(AV_CODEC_ID_H264);

        // start the frame processing thread
        VFP_thread = new std::thread ([this]() {run_VFP();} );

        while(!VFPrunning)
        {
           std::this_thread::sleep_for (std::chrono::milliseconds(20));
        }

	// start up the UDP packet receiver thread
      	VDR_thread = new std::thread([this] () {run_VDR();} );

	while(!VDRrunning)
	{
           std::this_thread::sleep_for (std::chrono::milliseconds(20));
	}

        ::printf("VideoFrameProcessor initialized\n");fflush(NULL);
  }

VideoFrameProcessor::~VideoFrameProcessor() throw ()
{
	printf ("VideoFrameProcessor - terminating\n");fflush(NULL);

	close (sockfd);
	UDPVideoUP = false;
	try
	{
		VDR_thread->join();
	}

	catch (int ex)
	{
		std::cout << "~VideoFrameProcessor - exception :" << ex << std::endl;fflush(NULL);
	}

	// Cleanup
	av_parser_close(parser);
}
  
void VideoFrameProcessor::run_VDR()
{
	FrameData frameData;
	unsigned int packet_count;
	unsigned int pos;

 	std::cout << "VideoFrameProcessor started\n";
	unsigned int payload_size = 65536;
	size_t receive_size;
	bool first_keyframe = false;

	VDRrunning = true;
	AVPacket packet;

	av_init_packet(&packet);
                
	int in_len;
	unsigned char * in_data;
	int len = 0;

	// Video packet processing loop
	// receive UDP packets, reassemble frame packets and send them on
	while (UDPVideoUP)
	{
		unsigned char packetdata[65536];
		socklen_t  soc_len = sizeof(cliaddr);  //len is value/resuslt

                receive_size = recvfrom(sockfd, (char *)packetdata, payload_size,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr,
					&soc_len);

		in_len = receive_size;
		in_data = packetdata;

		// rebuild frames from the UDP packets that are arriving
		while (in_len)
		{
			len = av_parser_parse2(parser,
						pCodecCtxH264,
						&(packet.data),
						&(packet.size),
						in_data,
						in_len,
						0,
						0,
						AV_NOPTS_VALUE);
			in_data += len;
			in_len  -= len;
			
			// have a packet, prepare it for processing
			if (packet.size != 0)
			{
				// annotate key frames
				if (parser->key_frame == 1 ||
					(parser->key_frame == -1 &&
					 parser->pict_type == AV_PICTURE_TYPE_I))
					packet.flags |= AV_PKT_FLAG_KEY;

        			if ((parser->key_frame == -1) &&
				    (parser->pict_type == AV_PICTURE_TYPE_NONE) &&
				    (packet.flags & AV_PKT_FLAG_KEY))
					packet.flags |= AV_PKT_FLAG_KEY;

				if(parser->key_frame)
					first_keyframe = true;

				// can only process after we receive a keyframe
				if(first_keyframe)
				{
					unsigned char * new_data;

					new_data = (unsigned char *) av_malloc(packet.size);
					memcpy(new_data, packet.data, packet.size);
					packet.data = new_data;	
 					addFrametoList (packet);	
				}
				av_init_packet (&packet); 
				packet.data = NULL;
			}
		}
	}
        
	std::cout << "VideoFrameProcessor stopped\n";
}

bool VideoFrameProcessor::getpFramefromList (cv::OutputArray image)
{
	while(NextFrame == NULL)
	{    
		// wait until we get something in the vector
		// BUG - need to handle the case where input stops
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
		//return false;
       }
	VidFrameMutex.lock();
	AVFrame dst;
        cv::Mat m;

	image.create(NextFrame->height,
			NextFrame->width,
			CV_8UC3);
#if 1
       ::printf("VideoFrameProcessor::getpFramefromList: %d w: %d dataptr %x linesize[0] %d linesize[1] %d linesize[2] %d\n",
                        NextFrame->height,
                        NextFrame->width,
                        NextFrame->data[0],
                        NextFrame->linesize[0],
                        NextFrame->linesize[1],
                        NextFrame->linesize[2]);fflush(NULL);
#endif
	cv::Mat image2 = image.getMat();

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

void VideoFrameProcessor::addpFrametoList(AVFrame* newFrame)
{

	VidFrameMutex.lock();

	// make sure the requestor receives the most recent frame
	// so we have only a one frame queue
	if(NextFrame != NULL)
	{
		av_free(NextFrame);
	}
	NextFrame = newFrame;
#if 0
	::printf( "VideoFrameProcessor::addpFrametoList h: %d  w: %d dataptr %x\n ",
                             newFrame->height, newFrame->width, newFrame->data[0]);fflush(NULL);
#endif
	newFrame = NULL;
	VidFrameMutex.unlock();
}

void  VideoFrameProcessor::addFrametoList(AVPacket frameData)
{
	VidMutex.lock();

	// When a new Key Frame arrives, flush the frame queue.
	// This way the most recent frame is sent to the codac
	if((frameData.flags & AV_PKT_FLAG_KEY) == AV_PKT_FLAG_KEY )
	{
		std::queue<AVPacket> empty;
		std::swap(FrameDataVector, empty);
	}
	FrameDataVector.push(frameData);
	VidMutex.unlock(); 
#if 0
	::printf("::addFrametoList -->Packet added to list frameData.data %x size %d\n",
		frameData.data,
		frameData.size);fflush(NULL);
#endif
}

void VideoFrameProcessor::receiverDone()
{
	waitingForReceiver = false;
}
  
void VideoFrameProcessor::run_VFP()
{
	int FrameFinished;
	static AVPacket packet;
	static struct SwsContext * img_convert_ctx = NULL;

	// setup the datastructures for the codes (move this stuff somewhere else...

	::printf("VideoFrameProcessor::run - Starting\n"); fflush(NULL);

	waitingForReceiver = true;
	VFPrunning = true;
 
	while(1)
	{
		while((FrameDataVector.empty())/* ||waitingForReceiver*/)
		{
			// wait until we get something in the vector
			std::this_thread::sleep_for (std::chrono::milliseconds(10));
		}
 
		VidMutex.lock();
		{
			packet = FrameDataVector.front();
			FrameDataVector.front().data = NULL;
			FrameDataVector.pop();
		}
		VidMutex.unlock();

		// got a frame, now decode it...
		{

			AVFrame* pFrame = av_frame_alloc();

			::printf("VideoFrameProcessor::run_VFP new packet data %x size %d\n",
					packet.data,
					packet.size);fflush(NULL);
			int avresult = avcodec_decode_video2(pCodecCtxH264, pFrame, &FrameFinished, &packet);

			//int avresult = decode(pCodecCtxH264, pFrame, &FrameFinished, &packet);

                	::printf("VideoFrameProcessor::run_VFP-->Decoding complete FrameFinished: %d avresult: %d\n",
					FrameFinished,
					avresult);fflush(NULL);

			if(FrameFinished == 0)    //  no output this time arounf
			{
				::printf("-->frame not decoded\n");fflush(NULL);
				continue;
			}	
			else                     // convert the image into something the KIPR SW can understand
			{
				::printf("VideoFrameProcessor::run_VFP data %x pFrame->linesize[0] %d linesize[1] %d width %d height %d key_frame %d\n",
						pFrame->data[0],
						pFrame->linesize[0],
						pFrame->linesize[1],
						pFrame->width,
						pFrame->height,
						pFrame->key_frame);fflush(NULL);
		
				if(pFrame->data[0] == NULL)
				{	
					::printf("VideoFrameProcessor_VFP::run ***Warning*** pointer to frame is NULL\n");fflush(NULL);
					continue;
				}

				// pass it on to the consumer
				addpFrametoList(pFrame);
        		}
		}
       std::this_thread::sleep_for (std::chrono::milliseconds(20));
	}
}


UDPVideo::UDPVideo(const char* DroneIpAddress, const short unsigned int DronePort, const int destw, const int desth)
{
	UDPOpened = false;
	NextFrame = NULL;
	DataReceiver = new VideoFrameProcessor(DroneIpAddress, DronePort, destw, desth);
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
	retval = DataReceiver->getpFramefromList(image);

	return retval;
} 

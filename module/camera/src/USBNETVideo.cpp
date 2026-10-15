#include "USBNETVideo.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "KIPRnet.hpp"

#include <unistd.h>
#include <poll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>


#include <opencv2/core/utils/logger.hpp>

#define DEST_PIXEL_FMT AV_PIX_FMT_BGR24
#define AI_CAMERA_H 720
#define AI_CAMERA_W 1280
#define UDP_RCV_TIMEOUT 100000 // in microseconds

#define FRAME_TIMEOUT 5000 // in milliseconds

#define FRAME_HEIGHT 720
#define FRAME_WIDTH

#define BUF_LEN 65540
#define PACK_SIZE 4096

#define AI_CAMERA_VIDEO_PORT 9000
#define AI_CAMERA_COMMAND_PORT 5556
#define AI_CAMERA_IP "192.168.1.2"

//#define DEBUG_USBNET 1
#include <ostream>
using namespace std;
using namespace cv;

//#define VDTDEBUG 1

void hexDump(char *desc, void *addr, int len);

UsbFrameProcessor::UsbFrameProcessor(
	const char *drone_ip_address, const short unsigned int drone_port,
	const int destw, const int desth)
{
	// todo: allow caller to set ip address

/*DEBUG*/
std::cout << cv::getBuildInformation() << std::endl;
cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);

	m_udp_video_up = true;
	m_vdr_running = false;

	m_ldestw = destw;
	m_ldesth = desth;

	m_next_object_list = nullptr;

	// start up the UDP packet receiver thread
	m_vdr_thread = new std::thread([this]()
   	                        { run_vdr(); });

  	while (!m_vdr_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	std::cout << "UsbFrameProcessor initialized" << std::endl;
}

	UsbFrameProcessor::~UsbFrameProcessor() throw()
{
	std::cout << "UsbFrameProcessor - terminating" << std::endl;

	m_udp_video_up = false;
	try
	{
		m_vdr_thread->join();
	}
	catch (int ex)
	{
		std::cout << "~UsbFrameProcessor - exception :" << ex
			<< std::endl
			<< std::flush;
 	}
	// Cleanup
	std::cout << "UsbFrameProcessor - terminated" << std::endl << std::flush;
}

void UsbFrameProcessor::run_vdr()
{

	std::cout << "UsbFrameProcessor started - run_vdr" << std::endl;

	// Video packet processing loop
	// receive UDP packets, reassemble frame packets and send them on
	m_vdr_running = true;

        char buffer[BUF_LEN]; // Buffer for echo string
        int recvMsgSize; // Size of received message
        string sourceAddress; // Address of datagram source
        unsigned short sourcePort; // Port of datagram source

	// set up the udp socket

	if ((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "socket creation failed" << std::endl
		<< std::flush;
		return;
	}

	memset(&m_servaddr, 0, sizeof(m_servaddr));

	// Filling server information
	m_servaddr.sin_family = AF_INET;
	m_servaddr.sin_port = htons(AI_CAMERA_VIDEO_PORT);
	m_servaddr.sin_addr.s_addr = inet_addr("192.168.1.1");

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

	clock_t last_cycle = clock();

	while (true) {
		size_t image_size;
start_block:
		if(!m_udp_video_up)
			break;
		// Block until receive message from a client

		do {
			socklen_t soc_len = sizeof(m_cliaddr); // len is value/resuslt
			recvMsgSize = recvfrom(m_sockfd, (char *)buffer, BUF_LEN, 0,
			(struct sockaddr *)&m_cliaddr, &soc_len);

			if ((int)recvMsgSize < 0)
			{
				std::cout << "run_vdr-frame hdr: " << strerror(errno) << std::endl;
				goto start_block;
			}
		} while (recvMsgSize == PACK_SIZE);
header_found:
		int total_pack = ((int *) buffer)[0];

		image_size = total_pack;
		total_pack = 1 + (image_size - 1) / PACK_SIZE;

		cout << "expected number of packets: " << total_pack << " HeaderSize " << recvMsgSize << " image_size " << image_size << endl;
		char * longbuf = new char[total_pack * PACK_SIZE];
		
		char * object_list;
		int object_count;

		if(recvMsgSize > (2*sizeof(int)))
		{
			memcpy(&object_count, buffer + sizeof(int), sizeof(int));
			object_list = new char[sizeof(int) + (object_count * sizeof(object_detection))];
			memcpy(object_list, buffer + sizeof(int),
				sizeof(int) + (object_count * sizeof(object_detection)));
		}
		else object_list = nullptr;

		for (int i = 0; i < total_pack; i++) {

		socklen_t soc_len = sizeof(m_cliaddr); // len is value/resuslt
		recvMsgSize = recvfrom(m_sockfd, (char *)buffer, BUF_LEN, 0,
				(struct sockaddr *)&m_cliaddr, &soc_len);
		if ((int)recvMsgSize < 0)
		{
			std::cout << "run_vdr-frame element: " << strerror(errno) << std::endl;
			free(longbuf);
			longbuf = nullptr;
			goto start_block;
		}
		if (recvMsgSize != PACK_SIZE) {
			cerr << "Received unexpected size pack:" << recvMsgSize << endl << std::flush;
			free(longbuf);
			longbuf = nullptr;
			goto header_found;   // must be start of a new block
                }
		memcpy( & longbuf[i * PACK_SIZE], buffer, recvMsgSize);
	}

	//add_pframe_to_list(frame, object_list);

	add_pframe_to_list(longbuf, image_size, object_list);

	}
	
	close(m_sockfd);
	std::cout << "UsbFrameProcessor - run_vdr stopped" << std::endl;
}

#define FRAME_SLEEP_TIME 10 // in milliseconds
bool UsbFrameProcessor::get_pframe_from_list(cv::OutputArray image, char* & object_list)
{
	unsigned long wait_time = 0;
	char * longbuf;
	size_t image_size;
tryagain:
	while (!m_new_frame)
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

	image_size = m_next_image_size;
	object_list = m_next_object_list;
	longbuf = m_next_longbuf;
	m_next_longbuf = nullptr;
	m_next_object_list = nullptr;
	m_new_frame = false;
	m_vid_frame_mutex.unlock();


size_t maxlen = 3;
        Mat frame = imdecode(std::vector<uchar>(longbuf, longbuf+image_size), IMREAD_COLOR|IMREAD_ANYDEPTH|IMREAD_IGNORE_ORIENTATION);

        if(frame.data == NULL)
        {
                cerr << "decode error - frame data is null" << endl;
                free(longbuf);
		longbuf = nullptr;
		goto tryagain;
        }
        image.create(frame.rows, frame.cols, CV_8UC3);
        frame.copyTo(image.getMatRef());

        free(longbuf);

	return true;
}

void UsbFrameProcessor::add_pframe_to_list(char * longbuf, size_t image_size, char * object_list)
{
	m_vid_frame_mutex.lock();

	// make sure the requestor receives the most recent frame
	// so we have only a one frame queue
	m_next_longbuf = longbuf;
	m_next_image_size = image_size;
	if(m_next_object_list != nullptr)
	free(m_next_object_list);
	m_next_object_list = object_list;
#ifdef DEBUGGETPFRAMW
	std::cout << "UsbFrameProcessor::add_pframe_to_list h: " << newFrame.rows
		<< " w: " << newFrame.cols << endl;
#endif
	m_new_frame = true;
	m_vid_frame_mutex.unlock();
}

UsbnetVideo::UsbnetVideo(const char *drone_ip_address,
                   const short unsigned int drone_port, const int destw,
                   const int desth)
{
	m_udp_opened = true;
	m_data_receiver =
		new UsbFrameProcessor(drone_ip_address, drone_port, destw, desth);
	m_object_list = nullptr;

	std::cout << "USB/UDP Video started" << std::endl;
}

UsbnetVideo::~UsbnetVideo()
{
	m_udp_opened = false;
	delete m_data_receiver;
	std::cout << "~UsbnetVideo done" << std::endl << std::flush;
}

bool UsbnetVideo::isOpened() const { return m_udp_opened; }

bool UsbnetVideo::read(cv::OutputArray image)
{
	bool retval = false;
	char * object_list;
	if (isOpened())
	{
		retval = m_data_receiver->get_pframe_from_list(image, object_list);

		m_object_list = object_list;
	}
  	return retval;
}

#define MAXRECVBUFF 256
#define AICAM_POLL_TIMEOUT 10000 /* in milliseconds */

// Source - https://stackoverflow.com/a/61960339
// Posted by Jay Sullivan, modified by community. See post 'Timeline' for change history
// Retrieved 2026-10-13, License - CC BY-SA 4.0

int connect_with_timeout(int sockfd, struct sockaddr_in *addr, socklen_t addrlen, unsigned int timeout_ms) {
    int rc = 0;
    // Set O_NONBLOCK
    int sockfd_flags_before;
    if((sockfd_flags_before=fcntl(sockfd,F_GETFL,0)<0)) return -1;
    if(fcntl(sockfd,F_SETFL,sockfd_flags_before | O_NONBLOCK)<0) return -1;
    // Start connecting (asynchronously)
    do {
        if (connect(sockfd, (struct sockaddr*) addr, addrlen)<0) {
            // Did connect return an error? If so, we'll fail.
            if ((errno != EWOULDBLOCK) && (errno != EINPROGRESS)) {
                rc = -1;
            }
            // Otherwise, we'll wait for it to complete.
            else {
                // Set a deadline timestamp 'timeout' ms from now (needed b/c poll can be interrupted)
                struct timespec now;
                if(clock_gettime(CLOCK_MONOTONIC, &now)<0) { rc=-1; break; }
                struct timespec deadline = { .tv_sec = now.tv_sec,
                                             .tv_nsec = now.tv_nsec + timeout_ms*100000l};
                // Wait for the connection to complete.
                do {
                    // Calculate how long until the deadline
                    if(clock_gettime(CLOCK_MONOTONIC, &now)<0) { rc=-1; break; }
                    int ms_until_deadline = (int)(  (deadline.tv_sec  - now.tv_sec)*1000l
                                                  + (deadline.tv_nsec - now.tv_nsec)/100000l);
                    if(ms_until_deadline<0) { rc=0; break; }
                    // Wait for connect to complete (or for the timeout deadline)
                    struct pollfd pfds[] = { { .fd = sockfd, .events = POLLOUT } };
                    rc = poll(pfds, 1, ms_until_deadline);
                    // If poll 'succeeded', make sure it *really* succeeded
                    if(rc>0) {
                        int error = 0; socklen_t len = sizeof(error);
                        int retval = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
                        if(retval==0) errno = error;
                        if(error!=0) rc=-1;
                    }
                }
                // If poll was interrupted, try again.
                while(rc==-1 && errno==EINTR);
                // Did poll timeout? If so, fail.
                if(rc==0) {
                    errno = ETIMEDOUT;
                    rc=-1;
                }
            }
        }
    } while(0);
    // Restore original O_NONBLOCK state
    if(fcntl(sockfd,F_SETFL,sockfd_flags_before)<0) return -1;
    // Success
    return rc;
}


char * ai_camera_command(char * command, int bufferlength)
{
	int flag = 1;
	int sockfd = -1;
	int connectreturn;
	struct sockaddr_in servaddr;
	int commandLen = strlen(command);   // Determine input length
	int bytes_sent;
	fd_set fdset;
	struct timeval tv;

	// Establish connection with the AI Camera server
/*debug*/ cout << "entering ai_camera_command" << endl << flush;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		cout << "socket creation failed" << endl;
		return NULL;
	}

	// used for connect timeout

//	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// assign IP, PORT

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(AI_CAMERA_IP);
	servaddr.sin_port = htons(AI_CAMERA_COMMAND_PORT);

	// connect  to the AI Camera command port
/*debug*/ cout << "connecting to server" << endl << flush;
	connectreturn = connect_with_timeout(sockfd, &servaddr, sizeof(servaddr), 4000);
       //connectreturn = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
/*DEBUG*/ cout << "connectreturn: " << connectreturn << endl;
	if(connectreturn < 0)
	{
		cout << "connection with AI server failed with error " << connectreturn << endl;
		perror("ai_camera_command connect");
		close(sockfd);
		return NULL;
	}

	// Send the string to the AI camera server

	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

/*DEBUG*/ cout << "sending ai_camera_command - " << command << " sockfd: " << sockfd << " status: " << connectreturn << std::endl << std::flush;

	bytes_sent = send(sockfd, command, commandLen, 0);

	cout << "ai_camera_command - command sent: " << command << endl << std::flush;

	int bytesReceived = 0;              // Bytes read on each recv()
	int totalBytesReceived = 0;         // Total bytes read
	cout << "ai_camera_command-Received: ";               // Setup to print the echoed string
        char * returnBuffer = (char *) malloc(MAXRECVBUFF);    // Buffer for echo string + \0

	int waitcount = 0;
	do
	{
		bytesReceived = recv(sockfd, returnBuffer, MAXRECVBUFF-1, MSG_DONTWAIT );
		if(bytesReceived < 0)
		{
			usleep(2500);
			waitcount++;
		}

	} while ( (bytesReceived < 0) && (waitcount < 5) );

	if (bytesReceived < 0) {
		perror("ai_camera_command - recv");
		close(sockfd);
		free(returnBuffer);
		return NULL;
	}
	returnBuffer[bytesReceived] = '\0';        // Terminate the string!
	cout << "->"<< returnBuffer << "<-" << std::endl << std::flush ;

	close(sockfd);
	return returnBuffer;
}

bool ai_camera_check()
{
	bool rtn;
	char * status = (char *) "status";

	char * returnBuffer = ai_camera_command(status, 7);

	if(returnBuffer == NULL)
	{
		return false;
	}

	if(strncmp(returnBuffer, "ready", 6) == 0)
		rtn = true;
	else
		rtn = false;

	free(returnBuffer);
	return rtn;
}


/*DEBUG - Remove before production */

void hexDump(char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

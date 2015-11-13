#ifndef _WIN32

#include "ardrone_video_p.hpp"
#include "ardrone_constants_p.hpp"

#include "uvlc_video_decoder_p.hpp"

#include "wallaby/util.h"
#include "wallaby/thread.hpp"

// ARM Workaround for labav.
// see: https://github.com/AutonomyLab/ardrone_autonomy/issues/1
#ifndef UINT64_C
#define UINT64_C(c) (c ## ULL)
#endif

// AR.Drone 2 video
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}
#include <opencv2/opencv.hpp>

using namespace Private;

ARDroneVideo::~ARDroneVideo()
{
}

class ARDrone1Video : public ARDroneVideo
{
public:
	bool start(const Address &address)
	{
		if(m_socket.isOpen()) end();
		m_address = address;
		if(m_address.isValid()) setupSocket(m_socket, m_address.port());
		return m_socket.isOpen();
	}
	
	bool isStarted() const
	{
		return m_address.isValid() && m_socket.isOpen();
	}
	
	bool wakeup()
	{
		if(!m_socket.isOpen()) {
#ifdef ARDRONE_DEBUG
			std::cout << "Failed to wakeup stream using invalid socket." << std::endl;
#endif
			return false;
		}
	
		const static char startCode[4] = { 0x01, 0x00, 0x00, 0x00 };
		if(m_socket.sendto(startCode, sizeof(startCode), m_address) != sizeof(startCode)) {
			perror("DroneController::wakeupStream -> sendto");
			return false;
		}
	
		return true;
	}
	
	bool fetch()
	{
		if(!m_socket.isOpen()) return false;
		
		ssize_t readLength = 0;
		unsigned char data[40000];
		if((readLength = m_socket.recv(data, 40000)) < 0 && errno != EAGAIN) {
			perror("DroneController::fetchVideo");
			return false;
		}

		if(readLength < 0) return true;
	
#ifdef ARDRONE_DEBUG
		std::cout << "Read " << readLength << " bytes from video stream" << std::endl;
#endif
		m_mutex.lock();
		UvlcVideoDecoder().decode(data, readLength, m_image);
		m_mutex.unlock();
		
		return true;
	}
	
	bool end()
	{
		m_address = Address();
		if(!m_socket.isOpen()) return false;
		m_socket.close();
		return true;
	}
	
	void latestImage(cv::Mat &image) const
	{
		m_mutex.lock();
		image = m_image.clone();
		m_mutex.unlock();
	}
	
private:
	bool setupSocket(Socket &socket, const unsigned short bindTo) const
	{
		socket.close();
		socket = Socket::udp();
	
		bool success = true;
		if(success) success &= socket.setBlocking(false);
		if(success) success &= socket.setReusable(true);
		if(success && bindTo) success &= socket.bind(bindTo);
		if(!success) {
			perror("setupSocket");
			socket.close();
		}
		
		return success;
	}
	
	Socket m_socket;
	Address m_address;
	cv::Mat m_image;
	mutable Mutex m_mutex;
};

typedef struct {
	// PaVE
	uint8_t signature[4];
	uint8_t version;
	uint8_t video_codec;
	uint16_t header_size;
	uint32_t payload_size;
	uint16_t encoded_stream_width;
	uint16_t encoded_stream_height;
	uint16_t display_width;
	uint16_t display_height;
	uint32_t frame_number;
	uint32_t timestamp;
	uint8_t total_chunks;
	uint8_t chunk_index;
	uint8_t frame_type;
	uint8_t control;
	uint32_t stream_byte_position_lw;
	uint32_t stream_byte_position_uw;
	uint16_t stream_id;
	uint8_t total_slices;
	uint8_t slice_index ;
	uint8_t header2_size;
	uint8_t header1_size;
	
	uint8_t reserved2[2];
	
	uint32_t advertised_size;
	
	// padding
	uint8_t reserved3[12];
	uint8_t reserved4[4];
} __attribute__ ((packed)) parrot_video_encapsulation_t;

typedef enum {
	FRAME_TYPE_UNKNNOWN = 0,
	FRAME_TYPE_IDR_FRAME, /* headers followed by I-frame */
	FRAME_TYPE_I_FRAME,
	FRAME_TYPE_P_FRAME,
	FRAME_TYPE_HEADERS
} parrot_video_encapsulation_frametypes_t;

class ARDrone2Video : public ARDroneVideo
{
public:
	ARDrone2Video()
		: m_codecCtx(0),
		m_frame(0),
		m_frameBgr(0),
		m_bufferBgr(0),
		lastFrame(0),
		m_state(WaitForIFrame)
	{
		initAv();
	}
	
	bool start(const Address &address)
	{
		m_address = address;
		if(!setupSocket(m_socket)) {
			std::cerr << "!!! Failed to setup video stream socket." << std::endl;
			return false;
		}
		if(!m_socket.connect(m_address)) {
			std::cerr << "!!! Failed to connect to video stream." << std::endl;
			perror("connect");
			m_socket.close();
			return false;
		}
		m_socket.setBlocking(false);

#ifdef ARDRONE_DEBUG
		std::cout << "Finding video decoder..." << std::endl;
#endif

		m_codecCtx = avcodec_alloc_context();
	        m_codecCtx->pix_fmt = PIX_FMT_YUV420P;
	        m_codecCtx->skip_frame = AVDISCARD_DEFAULT;
	        m_codecCtx->error_concealment = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
	        m_codecCtx->error_recognition = FF_ER_CAREFUL;
	        m_codecCtx->skip_loop_filter = AVDISCARD_DEFAULT;
	        m_codecCtx->workaround_bugs = FF_BUG_AUTODETECT;
	        m_codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	        m_codecCtx->codec_id = CODEC_ID_H264;
		m_codecCtx->skip_idct = AVDISCARD_DEFAULT;
		m_codecCtx->width = 640;
		m_codecCtx->height = 360;

		AVCodec *const codec = avcodec_find_decoder(m_codecCtx->codec_id);
		if(!codec) {
			std::cerr << "avcodec_find_decoder() failed." << std::endl;
			return false;
		}

#ifdef ARDRONE_DEBUG
		std::cout << "Opening video stream..." << std::endl;
#endif

		// Open codec
		if(avcodec_open2(m_codecCtx, codec, NULL) < 0) {
			std::cerr << "avcodec_open2() failed." << std::endl;
			return false;
		}
		
#ifdef ARDRONE_DEBUG
		std::cout << "Allocating video buffers" << std::endl;
#endif

		// Allocate video frames and a buffer
		m_frame = avcodec_alloc_frame();
		m_frameBgr = avcodec_alloc_frame();
		m_bufferBgr = (uint8_t *)av_mallocz(avpicture_get_size(PIX_FMT_BGR24,
			m_codecCtx->width, m_codecCtx->height) * sizeof(uint8_t));

		// Assign appropriate parts of buffer to image planes in m_frameBGR
		avpicture_fill((AVPicture *)m_frameBgr, m_bufferBgr, PIX_FMT_BGR24,
			m_codecCtx->width, m_codecCtx->height);
		
		m_convertCtx = sws_getContext(m_codecCtx->width, m_codecCtx->height, m_codecCtx->pix_fmt,
			m_codecCtx->width, m_codecCtx->height, PIX_FMT_BGR24, SWS_SPLINE, 0, 0, 0);
		
#ifdef ARDRONE_DEBUG
		std::cout << "ARDrone 2 video start success" << std::endl;
#endif
		
		m_img = cv::Mat(((m_codecCtx->height == 368) ? 360 : m_codecCtx->height),
			m_codecCtx->width, CV_8UC3);
		
		return true;
	}
	
	bool isStarted() const
	{
		return m_socket.isOpen();
	}
	
	bool wakeup()
	{
		if(!m_socket.isOpen()) {
#ifdef ARDRONE_DEBUG
			std::cout << "Failed to wakeup stream using invalid socket." << std::endl;
#endif
			return false;
		}
	
		const static char startCode[4] = { 0x01, 0x00, 0x00, 0x00 };
		if(m_socket.sendto(startCode, sizeof(startCode), m_address) != sizeof(startCode)) {
			perror("DroneController::wakeupStream -> sendto");
			return false;
		}
	
		return true;
	}
	
	bool fetch()
	{
		if(!isStarted()) return false;
		
		unsigned char part[40000];
		
		ssize_t readLength = 0;
		if((readLength = m_socket.recv(part, sizeof(part))) < 0 && errno != EAGAIN) {
			perror("DroneController::fetchVideo");
			return false;
		}

		if(readLength < 0) return true;
		
		
		parrot_video_encapsulation_t header;
		memcpy(&header, part, sizeof(parrot_video_encapsulation_t));
		// std::cout << header->signature << std::endl;
		if(strncmp((const char *)header.signature, "PaVE", 4)) {
#ifdef ARDRONE_DEBUG
			std::cout << "Invalid header" << std::endl;
#endif
			return true;
		}
		
		// printPave(header, std::cout);
		
		if(m_state == Normal && header.frame_number != lastFrame + 1) {
			m_state = WaitForIFrame;
#ifdef ARDRONE_DEBUG
			std::cout << "FRAME MISSED (got " << header.frame_number << ", expected" << (lastFrame + 1) <<  ")" << std::endl;
#endif
			return true;
		} else if(m_state == WaitForIFrame && (header.frame_type == FRAME_TYPE_IDR_FRAME
			|| header.frame_type == FRAME_TYPE_I_FRAME)) {
#ifdef ARDRONE_DEBUG
			std::cout << "Got our I Frame" << std::endl;
#endif
			m_state = Normal;
		} else if(m_state == WaitForIFrame) {
			return true;
		}
		lastFrame = header.frame_number;
		
		
		size_t read = 0;
		
		// Is it bad to assume things? Yes. It is *computationally fast* to assume things? Yes.
		unsigned char payload[40000];
		
		// Copy over part of the payload that was sent with the header
		memcpy(payload, part + header.header_size, readLength - header.header_size);
		read += readLength - header.header_size;
		
		double lastRead = seconds();
		while(read < header.payload_size && seconds() - lastRead < 0.1) {
#ifdef ARDRONE_DEBUG
			std::cout << read << " of " << header.payload_size << std::endl;
#endif
			if((readLength = m_socket.recv(payload + read, header.payload_size - read)) < 0 && errno != EAGAIN) {
				perror("DroneController::fetchVideo");
				return false;
			}
			if(readLength < 0) {
				msleep(10);
				continue;
			}
			read += readLength;
			lastRead = seconds();
		}
		
	
#ifdef ARDRONE_DEBUG
		std::cout << "Read " << read << " bytes from video stream" << std::endl;
#endif
		
		adjustDimensions(header);
		
		AVPacket packet;
		packet.data = payload;
		packet.size = header.payload_size;
		
		int done = 0;
		
		if(avcodec_decode_video2(m_codecCtx, m_frame, &done, &packet) < 0) {
#ifdef ARDRONE_DEBUG
			std::cout << "Didn't decode frame" << std::endl;
#endif
			return true;
		}
		
		if(!done) return true;
		
#ifdef ARDRONE_DEBUG
		static unsigned i = 0;
		std::cout << "Got video frame from AR.Drone 2 (num frames: " << ++i << ")" << std::endl;
#endif
	
		sws_scale(m_convertCtx, (const uint8_t *const *)m_frame->data, m_frame->linesize,
			0, m_codecCtx->height, m_frameBgr->data, m_frameBgr->linesize);

		m_mutex.lock();
		
#ifdef ARDRONE_DEBUG
		std::cout << "Copying video stream to latest image buffer" << std::endl;
#endif
		memcpy(m_img.ptr(), m_frameBgr->data[0], m_codecCtx->width * ((m_codecCtx->height == 368)
			? 360 : m_codecCtx->height) * sizeof(uint8_t) * 3);
		
		m_mutex.unlock();
		
		return true;
	}
	
	bool end()
	{
		if(!isStarted()) return false;
		
		if(m_frame) {
			av_free(m_frame);
			m_frame = 0;
		}

		if(m_frameBgr) {
			av_free(m_frameBgr);
			m_frameBgr = 0;
		}

		if(m_bufferBgr) {
			av_free(m_bufferBgr);
			m_bufferBgr = 0;
		}

		if(m_codecCtx) {
			avcodec_close(m_codecCtx);
			m_codecCtx = 0;
		}
		
		m_socket.close();
		m_address = Address();
		
		return true;
	}
	
	void latestImage(cv::Mat &image) const
	{
		m_mutex.lock();
		image = m_img.clone();
		m_mutex.unlock();
	}
	
private:
	static void initAv()
	{
		if(s_init) return;
		avcodec_init();
		avcodec_register_all();
		av_register_all();
		s_init = true;
	}
	
	bool setupSocket(Socket &socket) const
	{
		socket.close();
		socket = Socket::tcp();
	
		bool success = true;
		if(success) success &= socket.setReusable(true);
		if(!success) {
			perror("setupSocket");
			socket.close();
		}
		
		return success;
	}
	
	void adjustDimensions(const parrot_video_encapsulation_t &pave)
	{
		if(m_codecCtx->width == pave.display_width
			&& m_codecCtx->height == pave.display_height) return;
		
#ifdef ARDRONE_DEBUG
		std::cout << "Adjusting AR.Drone 2 Video dimensions old = (" << m_codecCtx->width << ", "
			<< m_codecCtx->height << "), new = (" << pave.display_width << "," << pave.display_height << ")" << std::endl;
#endif
		
		m_codecCtx->width = pave.display_width;
		m_codecCtx->height = pave.display_height;
		
		m_img = cv::Mat(((m_codecCtx->height == 368) ? 360 : m_codecCtx->height),
			m_codecCtx->width, CV_8UC3);
		m_bufferBgr = (uint8_t *)av_realloc(m_bufferBgr, m_img.rows * m_img.cols * m_img.elemSize() * sizeof(uint8_t));
		
		avpicture_fill((AVPicture *)m_frameBgr, m_bufferBgr, PIX_FMT_BGR24, m_codecCtx->width, m_codecCtx->height);

	        m_convertCtx = sws_getCachedContext(m_convertCtx, pave.display_width, pave.display_height,
			m_codecCtx->pix_fmt, pave.display_width, pave.display_height,
			m_codecCtx->pix_fmt, SWS_FAST_BILINEAR, 0, 0, 0);
	}
	
	void printPave(const parrot_video_encapsulation_t &pave, std::ostream &o) {
		using namespace std;
		o << "PaVE Dump: " << endl;
		o << "\tVersion: " << (uint32_t)pave.version << endl;
		o << "\tVideo Codec: " << (uint32_t)pave.video_codec << endl;
		o << "\tHeader Size: " << (uint32_t)pave.header_size << endl;
		o << "\tPayload Size: " << (uint32_t)pave.payload_size << endl;
		o << "\tEncoded Size: " << (uint32_t)pave.encoded_stream_width << ", "
			<< (uint32_t)pave.encoded_stream_height << endl;
		o << "\tDisplay Size: " << (uint32_t)pave.display_width
			<< (uint32_t)pave.display_height << endl;
		o << "\tFrame Number: " << pave.frame_number << endl;
		o << "\tTimestamp: " << (uint32_t)pave.timestamp << endl;
		o << "\tTotal Chunks: " << (uint32_t)pave.total_chunks << endl;
		o << "\tChunk Index: " << (uint32_t)pave.chunk_index << endl;
		o << "\tFrame Type: " << (uint32_t)pave.frame_type << endl;
		o << "\tControl: " << (uint32_t)pave.control << endl;
		o << "\tStream Byte Position: " << pave.stream_byte_position_lw << ", "
			<< pave.stream_byte_position_uw << endl;
		o << "\tStream ID: " << (uint32_t)pave.stream_id << endl;
		o << "\tTotal Slices: " << (uint32_t)pave.total_slices << endl;
		o << "\tSlice Index: " << (uint32_t)pave.slice_index << endl;
		o << "\tHeader2 Size: " << (uint32_t)pave.header2_size << endl;
		o << "\tHeader1 Size: " << (uint32_t)pave.header1_size << endl;
		o << "\tAdvertised Size: " << (uint32_t)pave.advertised_size << endl;
	}
	
	Socket m_socket;
	Address m_address;
	static bool s_init;
	AVCodecContext *m_codecCtx;
	AVFrame *m_frame;
	AVFrame *m_frameBgr;
	SwsContext *m_convertCtx;
	uint8_t *m_bufferBgr;
	cv::Mat m_img;
	uint32_t lastFrame;
	
	enum {
		Normal,
		WaitForIFrame
	} m_state;
	
	mutable Mutex m_mutex;
};

bool ARDrone2Video::s_init = false;

ARDroneVideo *ARDroneVideoFactory::video(const ARDrone::Version version)
{
	switch(version) {
	case ARDrone::V1: return new ARDrone1Video();
	case ARDrone::V2: return new ARDrone2Video();
	default:;
	}
	return 0;
}

#endif

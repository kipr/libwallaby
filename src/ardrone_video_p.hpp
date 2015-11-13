#ifndef _WIN32

#ifndef _ARDRONE_VIDEO_P_HPP_
#define _ARDRONE_VIDEO_P_HPP_

#include "wallaby/ardrone.hpp"
#include "wallaby/socket.hpp"
#include <opencv2/core/core.hpp>

namespace Private
{
	class ARDroneVideo
	{
	public:
		virtual ~ARDroneVideo();
		virtual bool start(const Address &address) = 0;
		virtual bool isStarted() const = 0;
		virtual bool wakeup() = 0;
		virtual bool fetch() = 0;
		virtual bool end() = 0;
		virtual void latestImage(cv::Mat &image) const = 0;
	};
	
	class ARDroneVideoFactory
	{
	public:
		static ARDroneVideo *video(const ARDrone::Version version);
	};
}

#endif

#endif

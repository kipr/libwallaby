/*
 * camera.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_CAMERA_HPP_
#define INCLUDE_WALLABY_CAMERA_HPP_

 /*!
 * \file camera.hpp
 * \brief Classes for working with the system camera.
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup camera Camera
 */

#include "geom.hpp"
#include "color.hpp"
#include "config.hpp"
#include "export.h"
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#ifndef WIN32
#include <sys/time.h>
#else
#define NOMINMAX
#include <time.h>
#include <winsock2.h>
#endif

#include <opencv2/core/core.hpp>

// These keys are used in the config files loaded by
// Camera::Device
#define CAMERA_GROUP ("camera")
#define CAMERA_NUM_CHANNELS_KEY ("num_channels")
#define CAMERA_CHANNEL_GROUP_PREFIX ("channel_")
#define CAMERA_CHANNEL_TYPE_KEY ("type")

#define CAMERA_CHANNEL_TYPE_HSV_KEY ("hsv")
#define CAMERA_CHANNEL_TYPE_QR_KEY ("qr")

namespace cv
{
	class VideoCapture;
}

namespace Camera
{
	class Device;

	class EXPORT_SYM Object
	{
	public:
		Object(const Point2<unsigned> &centroid,
			const Rect<unsigned> &boundingBox,
			const double &confidence, const char *data = 0,
			const size_t &dataLength = 0);
		Object(const Object &rhs);
		~Object();
#ifndef SWIG
		const Point2<unsigned> &centroid() const;
		const Rect<unsigned> &boundingBox() const;
#endif

		const double confidence() const;
		const char *data() const;
		const size_t dataLength() const;

	private:
		Point2<unsigned> m_centroid;
		Rect<unsigned> m_boundingBox;
		double m_confidence;
		char *m_data;
		size_t m_dataLength;
	};

	typedef std::vector<Object> ObjectVector;

	class EXPORT_SYM ChannelImpl
	{
	public:
		ChannelImpl();
		virtual ~ChannelImpl();

		void setImage(const cv::Mat &image);
		ObjectVector objects(const Config &config);

	protected:
		virtual void update(const cv::Mat &image) = 0;
		virtual ObjectVector findObjects(const Config &config) = 0;

	private:
		bool m_dirty;
		cv::Mat m_image;
	};

	class EXPORT_SYM ChannelImplManager
	{
	public:
		virtual ~ChannelImplManager();
		virtual void setImage(const cv::Mat &image) = 0;
		virtual ChannelImpl *channelImpl(const std::string &name) = 0;
	};

	class EXPORT_SYM DefaultChannelImplManager : public ChannelImplManager
	{
	public:
		DefaultChannelImplManager();
		~DefaultChannelImplManager();

		virtual void setImage(const cv::Mat &image);
		virtual ChannelImpl *channelImpl(const std::string &name);

	private:
		std::map<std::string, ChannelImpl *> m_channelImpls;
	};

	class EXPORT_SYM Channel
	{
	public:
		Channel(Device *device, const Config &config);
		~Channel();

		void invalidate();

#ifndef SWIG
		const ObjectVector *objects() const;
#endif

		Device *device() const;

		/**
		 * Do not call this method unless you know what you are doing!
		 */
		void setConfig(const Config &config);

	private:
		Device *m_device;
		Config m_config;
		mutable ObjectVector m_objects;
		ChannelImpl *m_impl;
		mutable bool m_valid;
	};

	typedef std::vector<Channel *> ChannelPtrVector;

	class EXPORT_SYM ConfigPath
	{
	public:
		static std::string extension();

		static void setBasePath(const std::string &path);
		static std::string path(const std::string &name = std::string());
		static std::string defaultPath();
		static std::string defaultConfigPath();
		static void setDefaultConfigPath(const std::string &name);

	private:
		static std::string s_path;
	};

	class EXPORT_SYM InputProvider
	{
	public:
		virtual ~InputProvider();
		virtual bool open(const int number) = 0;
		virtual bool isOpen() const = 0;
		virtual void setWidth(const unsigned width) = 0;
		virtual void setHeight(const unsigned height) = 0;
		virtual bool next(cv::Mat &image) = 0;
		virtual bool close() = 0;
	};

	class EXPORT_SYM UsbInputProvider : public InputProvider
	{
	public:
		UsbInputProvider();
		~UsbInputProvider();

		virtual bool open(const int number);
		virtual bool isOpen() const;
		virtual void setWidth(const unsigned width);
		virtual void setHeight(const unsigned height);
		virtual bool next(cv::Mat &image);
		virtual bool close();

	private:
		cv::VideoCapture *m_capture;
	};

	class EXPORT_SYM Device
	{
	public:
		Device(InputProvider *const inputProvider);
		~Device();

		bool open(const int number = 0);
		bool isOpen() const;
		bool close();
		bool update();

		void setWidth(const unsigned width);
		void setHeight(const unsigned height);

		unsigned width() const;
		unsigned height() const;
#ifndef SWIG
		const ChannelPtrVector &channels() const;
#endif

		InputProvider *inputProvider() const;
		const cv::Mat &rawImage() const;

		void setConfig(const Config &config);
		const Config &config() const;

		void setChannelImplManager(ChannelImplManager *channelImplManager);
		ChannelImplManager *channelImplManager() const;

		const unsigned char *bgr() const;

	private:
		void updateConfig();

		InputProvider *const m_inputProvider;
		Config m_config;
		ChannelPtrVector m_channels;
		ChannelImplManager *m_channelImplManager;
		cv::Mat m_image;
		timeval m_lastUpdate;

		mutable unsigned char *m_bgr;
		mutable unsigned m_bgrSize;
	};

	/**
	 * Retrieve the device singleton that backs the libkovan C camera API
	 */
	Camera::Device *cDevice();
}

#endif /* INCLUDE_WALLABY_CAMERA_HPP_ */

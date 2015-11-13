/**************************************************************************
 *  Copyright 2013 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

/**
 * \author Braden McDorman
 */

#ifndef _WIN32

#include "wallaby/ardrone.hpp"
#include "wallaby/thread.hpp"
#include "wallaby/socket.hpp"
#include "wallaby/util.h"
#include "ardrone_constants_p.hpp"
#include "ardrone_video_p.hpp"
#include "network_interface_p.hpp"
#include "ardrone_emergency_stop_p.hpp"

#include "wallaby/debug.h"

#include <opencv2/opencv.hpp>

#include <stdio.h>

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <errno.h>
#include <climits>

#include <stack>

#include <stdint.h>

using namespace Private;

#define ARDRONE_FLOAT_TO_UNSIGNED(x) (*(unsigned *)&(x))

struct navdata_option_t
{
	// Navdata block ('option') identifier
	uint16_t tag;
	
	// set this to the size of this structure
	uint16_t size;
	
	uint8_t data[];
} __attribute__((packed));

struct navdata_t {
	// Always set to NAVDATA_HEADER
	uint32_t header;

	// Bit mask built from def_ardrone_state_mask_t
	uint32_t ardrone_state;

	// Sequence number, incremented for each sent packet
	uint32_t sequence;
	uint32_t vision_defined;

	navdata_option_t options[];
} __attribute__((packed));

struct navdata_mat3_t
{
	float _00;
	float _01;
	float _02;
	float _10;
	float _11;
	float _12;
	float _20;
	float _21;
	float _22;
} __attribute__((packed));

struct navdata_vec3_t
{
	float _0;
	float _1;
	float _2;
} __attribute__((packed));

struct navdata_demo_t
{
	// Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum.
	uint32_t ctrl_state;
	
	// battery voltage filtered (mV)
	uint32_t vbat_flying_percentage;

	// UAV's pitch in milli-degrees
	float pitch;
	
	// UAV's roll in milli-degrees
	float roll;
	
	// UAV's yaw in milli-degrees
	float yaw;

	// UAV's altitude in centimeters
	int32_t altitude;

	// UAV's estimated linear velocity
	navdata_vec3_t velocity;

	uint32_t frameIndex;
	
	navdata_mat3_t detection_cam_rotation;
	navdata_vec3_t detection_cam_translation;
	uint32_t tagIndex;
	uint32_t detectionType;
	
	navdata_mat3_t drone_cam_rotation;
	navdata_vec3_t drone_cam_translation;
} __attribute__((packed));

struct navdata_raw_t
{
	uint16_t accelX;
	uint16_t accelY;
	uint16_t accelZ;
	
	uint16_t gyroX;
	uint16_t gyroY;
	uint16_t gyroZ;
	
	int16_t gyroX_110;
	int16_t gyroY_110;
	
	uint32_t batteryMilliVolt;
	
	uint16_t usEchoStart;
	uint16_t usEchoEnd;
	uint16_t usEchoAssociation;
	uint16_t usEchoDistance;
	
	uint16_t usCurveTime;
	uint16_t usCurveValue;
	uint16_t usCurveRef;
	
	uint16_t echoFlagIni;
	uint16_t echoNum;
	uint16_t echoSum;
        
	int32_t altTemp;
} __attribute__((packed));

struct navdata_phys_t
{
	 float accelTemp;
	 uint16_t gyroTemp;
	 
	 navdata_vec3_t accel;
         navdata_vec3_t gyro;
	 
	 uint32_t alim3V3;
	 uint32_t vrefEpson;
	 uint32_t vrefIDG;
} __attribute__((packed));

/**
 * \class DroneSequencer
 * Provides an encapsulated positively incremented value. Also has some
 * helper methods. One of several attempts to clean up
 * DroneController
 */
class DroneSequencer
{
public:
	typedef unsigned sequence_t;
	
	DroneSequencer();
	
	/**
	 * Blocks until a given number of increments have occured
	 * \param times number of increments to block until
	 */
	void wait(const unsigned times = 1);
	
	/**
	 * Increments the sequencer by 1
	 */
	DroneSequencer::sequence_t next();
	
	/**
	 * Resets the sequencer to 0
	 */
	void reset();
	
private:
	sequence_t m_current;
};

DroneSequencer::DroneSequencer()
{
	reset();
}

void DroneSequencer::wait(const unsigned times)
{
	sequence_t current = m_current;
	while(current + times >= m_current) msleep(10);
}

DroneSequencer::sequence_t DroneSequencer::next()
{
	return ++m_current;
}

void DroneSequencer::reset()
{
	m_current = 0;
}

struct DroneCommand
{
	DroneCommand();
	
	char data[ARDRONE_MAX_CMD_LENGTH];
};

DroneCommand::DroneCommand()
{
	memset(data, 0, ARDRONE_MAX_CMD_LENGTH);
}

/**
 * \class VelocityIntegrator
 * \brief Calculates position from velocities and time.
 *
 * Velocity is expected to be per second.
 */
class VelocityIntegrator
{
public:
	VelocityIntegrator();
	
	void update(const Vec3f &velocity);
	void clear();
	
	Vec3f position() const;
private:
	Vec3f m_position;
	double m_lastUpdate;
};

VelocityIntegrator::VelocityIntegrator()
{
	clear();
}

void VelocityIntegrator::update(const Vec3f &velocity)
{
	const double currentTime = seconds();
	double delta = currentTime - m_lastUpdate;
	m_position.x += velocity.x * delta;
	m_position.y += velocity.y * delta;
	m_position.z += velocity.z * delta;
	m_lastUpdate = currentTime;
}

void VelocityIntegrator::clear()
{
	m_position = Vec3f();
	m_lastUpdate = seconds();
}

Vec3f VelocityIntegrator::position() const
{
	return m_position;
}

/**
 * \class DroneController
 * This is a beast of a class. This class could be considered a very porous mid-level
 * interface to the drone. DroneController is further abstracted by the very tight
 * ARDrone class. To understand the inner workings of this class, please refer to
 * the (awful) official AR.Drone communication spec in /useful
 */
class DroneController : public Thread
{
public:
	enum DoReturn {
		Success = 0,
		Error,
		NotReady
	};
	
	enum FetchReturn {
		Fetched = 0,
		NotFetched,
		FetchError
	};
	
	DroneController();
	~DroneController();
	
	void invalidate();
	
	void flatTrim();
	
	void land(const bool emergency = false);
	void takeoff(const bool emergency = true);
	
	void move(const float x, const float y, const float z, const float yaw);
	
	/**
	 * Requests that navdata be sent to our IP.
	 * We call this periodically.
	 */
	bool requestNavdata();
	
	void setVideoCodec(const unsigned id);
	
	/**
	 * Sends an AT*CONF command
	 */
	void configure(const char *const cmd, const char *const value);
	
	/**
	 * \see configure(const char *const, const char *const)
	 */
	void configure(const char *const cmd, unsigned value);
	
	void run();
	
	void stop();
	bool isStopped() const;
	
	void control(const unsigned mode);
	void sendMagic();
	
	void setOwner(const char *const mac);
	void setSsid(const char *const ssid);
	void setNavdataDemo(const bool mode);
	
	bool isControlWatchdog() const;
	bool isLowBatt() const;
	
	void resetControlWatchdog();
	void sendCommWatchdog();
	
	void setActiveCamera(const ARDrone::Camera camera);
	
	const navdata_t *latestNavdata() const;
	const navdata_demo_t *latestNavdataDemo() const;
	const navdata_raw_t *latestNavdataRawMeasures() const;
	
	bool writeNavdata(const char *const path) const;
	
	void setAtAddress(const Address &atAddress);
	void setNavdataAddress(const Address &atAddress);
	void setVideoAddress(const Address &atAddress);
	void setConfigAddress(const Address &configAddress);
	
	const Address &atAddress() const;
	const Address &navdataAddress() const;
	const Address &videoAddress() const;
	const Address &configAddress() const;
	
	bool isValid() const;
	bool isAtSocketValid() const;
	bool isNavdataSocketValid() const;
	bool isConfigSocketValid() const;
	
	void image(cv::Mat &image) const;
	
	bool isNotTalking() const;
	
	void clearPosition();
	Vec3f position() const;
	
	std::map<std::string, std::string> configuration(const double timeout = 3.0);
	
	void setVideoFrameRate(const unsigned videoFrameRate);
	
private:
	void pushCommand(const char *const command);
	void popCommand();
	void popAllCommands();
	void oneTimeCommand(const char *const command);
	bool sendCurrentCommand();
	
	DroneController::FetchReturn fetchNavdata();
	
	DoReturn doAt();
	DoReturn doNavdata();
	DoReturn doVideo();
	
	void linkNavdata();
	
	bool wakeupStream(Socket &socket, const Address &address);
	
	bool setupSocket(Socket &socket, const unsigned short bindTo = 0) const;
	
	Socket m_atSocket;
	double m_atLast;
	
	Socket m_navdataSocket;
	double m_navLast;
	double m_navReqLast;
	
	double m_videoFetchLast;
	double m_videoWakeupLast;
	
	Address m_atAddress;
	Address m_navdataAddress;
	Address m_videoAddress;
	Address m_configAddress;
	
	mutable Mutex m_mutex;
	mutable Mutex m_imageMutex;
	
	Mutex m_control;
	std::stack<DroneCommand> m_commandStack;
	DroneSequencer m_seq;
	bool m_stop;
	unsigned char m_navdata[65507];
	
	const navdata_demo_t *m_navdataDemo;
	const navdata_raw_t *m_navdataRawMeasures;
	
	bool m_cameraActivated;
	
	unsigned m_ticksSinceLastFetch;
	
	VelocityIntegrator m_velIntegrator;
	
	ARDrone::Version findVersion();
	ARDrone::Version m_currentVersion;
	
	ARDroneVideo *m_video;
	double m_videoRefreshRate;
};

DroneController::DroneController()
	: m_atLast(0.0),
	m_navLast(0.0),
	m_navReqLast(0.0),
	m_videoFetchLast(0.0),
	m_videoWakeupLast(0.0),
	m_stop(true),
	m_cameraActivated(false),
	m_video(0),
	m_videoRefreshRate(0.023)
{
	invalidate();
}

DroneController::~DroneController()
{
	if(m_video) m_video->end();
	delete m_video;
}

void DroneController::invalidate()
{
	setAtAddress(Address());
	setNavdataAddress(Address());
	setVideoAddress(Address());
	
	// Invalidate navdata by clearing header
	memset(m_navdata, 0, 8);
	m_ticksSinceLastFetch = UINT_MAX;
}

void DroneController::flatTrim()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u\r", ARDRONE_AT_FTRIM);
	oneTimeCommand(command);
}

void DroneController::land(const bool emergency)
{
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_REF, 0x11540000 | ((emergency ? 1 : 0) << 8));
	pushCommand(command);
}

void DroneController::takeoff(const bool emergency)
{
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_REF, 0x11540200 | ((emergency ? 1 : 0) << 8));
	pushCommand(command);
	msleep(1000);
}

void DroneController::move(const float x, const float y, const float z, const float yaw)
{
	unsigned ix = ARDRONE_FLOAT_TO_UNSIGNED(x);
	unsigned iy = ARDRONE_FLOAT_TO_UNSIGNED(y);
	unsigned iz = ARDRONE_FLOAT_TO_UNSIGNED(z);
	unsigned iyaw = ARDRONE_FLOAT_TO_UNSIGNED(yaw);
	
	const bool hoverMode = x == 0.0f && y == 0.0f
		&& z == 0.0f && yaw == 0.0f;
	
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u,%u,%u,%u,%u\r", ARDRONE_AT_PCMD,
		hoverMode ? 0 : 1,
		ix, iy, iz, iyaw);
	pushCommand(command);
}

bool DroneController::requestNavdata()
{
	if(!m_navdataSocket.isOpen()) return false;
	return wakeupStream(m_navdataSocket, m_navdataAddress);
}

void DroneController::setVideoCodec(const unsigned id)
{
	configure(ARDRONE_VIDEO_CODEC, id);
}

void DroneController::configure(const char *const cmd, const char *const value)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,\"%s\",\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG_IDS,
		ARDRONE_SESSION_ID, ARDRONE_USER_ID, ARDRONE_APPLICATION_ID);
	oneTimeCommand(command);
	
	sprintf(command, "%s%%u,\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG, cmd, value);
	oneTimeCommand(command);
}

void DroneController::configure(const char *const cmd, unsigned value)
{
	char vbuf[128];
	sprintf(vbuf, "%u", value);
	configure(cmd, vbuf);
}
	
void DroneController::run()
{
	m_stop = false;
	while(!m_stop) {
		m_mutex.lock();
		DroneController::DoReturn at = doAt();
		DroneController::DoReturn navdata = doNavdata();
		DroneController::DoReturn video = doVideo();
		m_mutex.unlock();
		
		if(at == DroneController::Error
			|| navdata == DroneController::Error
			// || video == DroneController::Error) {
		) {
			std::cerr << "AR.Drone internal error" << std::endl;
		}
		
		msleep(4);
	}
	m_stop = false;
	m_seq.reset();
	popAllCommands();
	if(m_video) m_video->end();
}

void DroneController::stop()
{
	m_mutex.lock();
	m_stop = true;
	m_mutex.unlock();
}

bool DroneController::isStopped() const
{
	return m_stop;
}

void DroneController::control(const unsigned mode)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u,0\r", ARDRONE_AT_CTRL, mode);
	oneTimeCommand(command);
}

void DroneController::sendMagic()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_PMODE, 2);
	oneTimeCommand(command);
	sprintf(command, "%s%%u,%u,%u,%u,%u\r", ARDRONE_AT_MISC,
		2, 20, 2000, 3000);
	oneTimeCommand(command);
}

void DroneController::setOwner(const char *const mac)
{
	configure(ARDRONE_NETWORK_OWNER_MAC, mac);
}

void DroneController::setSsid(const char *const ssid)
{
	configure(ARDRONE_NETWORK_SSID, ssid);
}

void DroneController::setNavdataDemo(const bool mode)
{
	configure(ARDRONE_NAVDATA_DEMO, mode ? ARDRONE_TRUE : ARDRONE_FALSE);
}

bool DroneController::isControlWatchdog() const
{
	const navdata_t *const latest = latestNavdata();
	if(!latest) return false;
	
	return latest->ardrone_state & ARDRONE_CTRL_WATCHDOG_MASK;
}

bool DroneController::isLowBatt() const
{
	const navdata_t *const latest = latestNavdata();
	if(!latest) return false;
	
	return latest->ardrone_state & ARDRONE_VBAT_LOW;
}

void DroneController::resetControlWatchdog()
{
	// control(ARDRONE_ACK_CONTROL_MODE);
}

void DroneController::sendCommWatchdog()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u\r", ARDRONE_AT_COMWDG);
	oneTimeCommand(command);
}

void DroneController::setActiveCamera(const ARDrone::Camera camera)
{
	m_mutex.lock();
	m_cameraActivated = camera != ARDrone::Off;
	
	if(!m_cameraActivated) {
		if(m_video) m_video->end();
		m_mutex.unlock();
		return;
	}
	
	if(m_video && m_videoAddress.isValid()) m_video->start(m_videoAddress);
	m_mutex.unlock();
	
	unsigned value = 0;
	switch(camera) {
	case ARDrone::Front:
		value = 0;
		break;
	case ARDrone::Bottom:
		value = 1;
		break;
	default: break;
	}
	
	configure(ARDRONE_VIDEO_CHANNEL, value);
}

const navdata_t *DroneController::latestNavdata() const
{
	const navdata_t *const ret = reinterpret_cast<const navdata_t *>(m_navdata);
	if(!ret || ret->header != ARDRONE_NAVDATA_HEADER) return 0;
	return ret;
}

const navdata_demo_t *DroneController::latestNavdataDemo() const
{
	return m_navdataDemo;
}

const navdata_raw_t *DroneController::latestNavdataRawMeasures() const
{
	return m_navdataRawMeasures;
}

bool DroneController::writeNavdata(const char *const path) const
{
	FILE *fp = fopen(path, "w");
	if(!fp) return false;
	fwrite(m_navdata, 1, sizeof(m_navdata), fp);
	fclose(fp);
	return true;
}

void DroneController::setAtAddress(const Address &atAddress)
{
	m_mutex.lock();
	m_atAddress = atAddress;
	
	if(m_atAddress.isValid()) setupSocket(m_atSocket);
	else m_atSocket.close();
	
	m_mutex.unlock();
}

void DroneController::setNavdataAddress(const Address &navdataAddress)
{
	m_mutex.lock();
	m_navdataAddress = navdataAddress;
	
	if(m_navdataAddress.isValid()) setupSocket(m_navdataSocket, m_navdataAddress.port());
	else m_navdataSocket.close();
	
	m_mutex.unlock();
}

void DroneController::setVideoAddress(const Address &videoAddress)
{
	if(m_video) m_video->end();
	delete m_video;
	m_video = ARDroneVideoFactory::video(m_currentVersion);
	
	m_mutex.lock();
	m_videoAddress = videoAddress;
	m_mutex.unlock();
	
	if(m_videoAddress.isValid()) setVideoCodec(m_currentVersion == ARDrone::V1
		? ARDRONE_VIDEO_UVLC_CODEC
		: ARDRONE_VIDEO_H264_360P_CODEC);
}

void DroneController::setConfigAddress(const Address &configAddress)
{
	m_mutex.lock();
	m_configAddress = configAddress;
	m_mutex.unlock();
	
	m_currentVersion = findVersion();
}

const Address &DroneController::atAddress() const
{
	return m_atAddress;
}

const Address &DroneController::navdataAddress() const
{
	return m_navdataAddress;
}

const Address &DroneController::videoAddress() const
{
	return m_videoAddress;
}

const Address &DroneController::configAddress() const
{
	return m_configAddress;
}

bool DroneController::isValid() const
{
	return isAtSocketValid() && isNavdataSocketValid();
}

bool DroneController::isAtSocketValid() const
{
	return m_atSocket.isOpen();
}

bool DroneController::isNavdataSocketValid() const
{
	return m_navdataSocket.isOpen();
}

void DroneController::image(cv::Mat &image) const
{
	if(!m_video || !m_video->isStarted()) return;
	m_video->latestImage(image);
}

bool DroneController::isNotTalking() const
{
	return m_ticksSinceLastFetch > 25;
}

void DroneController::clearPosition()
{
	m_mutex.lock();
	m_velIntegrator.clear();
	m_mutex.unlock();
}

Vec3f DroneController::position() const
{
	m_mutex.lock();
	Vec3f ret = m_velIntegrator.position();
	m_mutex.unlock();
	
	return ret;
}

void DroneController::pushCommand(const char *const command)
{
	DroneCommand container;
	strncpy(container.data, command, ARDRONE_MAX_CMD_LENGTH);
	
	m_mutex.lock();
	m_commandStack.push(container);
	m_mutex.unlock();
}

void DroneController::popCommand()
{
	m_mutex.lock();
	if(!m_commandStack.empty()) m_commandStack.pop();
	m_mutex.unlock();
}

void DroneController::popAllCommands()
{
	m_mutex.lock();
	while(!m_commandStack.empty()) m_commandStack.pop();
	m_mutex.unlock();
}

void DroneController::oneTimeCommand(const char *const command)
{
	pushCommand(command);
	m_seq.wait(2);
	popCommand();
}

DroneController::FetchReturn DroneController::fetchNavdata()
{
	ssize_t readLength = 0;
	char data[sizeof(m_navdata)];
	if((readLength = m_navdataSocket.recv(data, sizeof(data))) < 0 && errno != EAGAIN) {
		perror("DroneController::fetchNavdata");
		return DroneController::FetchError;
	}
	if(readLength < 0) return DroneController::NotFetched;
	
#ifdef ARDRONE_DEBUG
	std::cout << "Read " << readLength << " bytes from navdata stream" << std::endl;
#endif
	
	memcpy(m_navdata, data, sizeof(m_navdata));
	
	return DroneController::Fetched;
}

DroneController::DoReturn DroneController::doAt()
{
	if(!m_atSocket.isOpen()) return DroneController::NotReady;
	
	if(seconds() - m_atLast >= 0.03) {
		if(!sendCurrentCommand()) return DroneController::Error;
#ifdef ARDRONE_DEBUG
		if(seconds() - m_atLast >= 0.04) {
			std::cout << "WARNING: SLOW AT (" << (seconds() - m_atLast) << ")" << std::endl;
		}
#endif
		m_atLast = seconds();
	}
	
	return DroneController::Success;
}

DroneController::DoReturn DroneController::doNavdata()
{
	if(!m_navdataSocket.isOpen()) return DroneController::NotReady;
	
	if(seconds() - m_navLast >= 0.01) {
		// Frequently
		DroneController::FetchReturn ret = fetchNavdata();
		if(ret == DroneController::FetchError) return DroneController::Error;
	
		if(ret == DroneController::Fetched) {
			m_ticksSinceLastFetch = 0;
			linkNavdata();
		} else if(ret == DroneController::NotFetched && m_ticksSinceLastFetch < UINT_MAX) {
			++m_ticksSinceLastFetch;
		}
		m_navLast = seconds();
	}
	
	if(seconds() - m_navReqLast > 2.0) {
		if(!requestNavdata()) return DroneController::Error;
		m_navReqLast = seconds();
	}
	
	return DroneController::Success;
}

DroneController::DoReturn DroneController::doVideo()
{
	if(!m_video || !m_video->isStarted()) return DroneController::NotReady;
	
	if(seconds() - m_videoFetchLast > m_videoRefreshRate) {
		if(!m_video->fetch()) return DroneController::Error;
		m_videoFetchLast = seconds();
	}
	
	if(seconds() - m_videoWakeupLast > 0.9) {
		if(!m_video->wakeup()) return DroneController::Error;
		m_videoWakeupLast = seconds();
	}
	
	return DroneController::Success;
}

void DroneController::linkNavdata()
{
	// Clear old links
	m_navdataDemo = 0;
	m_navdataRawMeasures = 0;
	
	const navdata_t *const raw = reinterpret_cast<navdata_t *>(m_navdata);
	
	if(raw->header != ARDRONE_NAVDATA_HEADER) return;
	
	const unsigned char *position = reinterpret_cast<const unsigned char *>(&raw->options[0]);
	
	const navdata_option_t *current = reinterpret_cast<const navdata_option_t *>(position);
	while(current->tag != ARDRONE_NAVDATA_CKS_TAG) {
		if(current->tag == ARDRONE_NAVDATA_DEMO_TAG) {
			m_navdataDemo = reinterpret_cast<const navdata_demo_t *>(current->data);
			
			// TODO: Should this be moved?
			Vec3f vel(m_navdataDemo->velocity._0,
				m_navdataDemo->velocity._1,
				m_navdataDemo->velocity._2);
			m_velIntegrator.update(vel);
		}
		
		position += current->size;
		current = reinterpret_cast<const navdata_option_t *>(position);
	}
}

bool DroneController::wakeupStream(Socket &socket, const Address &address)
{
	if(!socket.isOpen()) {
#ifdef ARDRONE_DEBUG
		std::cout << "Failed to wakeup stream using invalid socket." << std::endl;
#endif
		return false;
	}
	
	const static char startCode[4] = { 0x01, 0x00, 0x00, 0x00 };
	if(socket.sendto(startCode, sizeof(startCode), address) != sizeof(startCode)) {
		perror("DroneController::wakeupStream -> sendto");
		return false;
	}
	
	return true;
}

bool DroneController::sendCurrentCommand()
{
	// Nothing to do.
	if(m_commandStack.empty()) return true;
	
	char commWdg[ARDRONE_MAX_CMD_LENGTH];
	sprintf(commWdg, "%s%u\r", ARDRONE_AT_COMWDG, m_seq.next());
	
#ifdef ARDRONE_DEBUG
	std::cout << "Sending watchdog" << std::endl;
#endif
	
	if(m_atSocket.sendto(commWdg, strlen(commWdg), m_atAddress) < 0) {
		perror("DroneController::run -> sendto");
		return false;
	}
	
	char realCommand[ARDRONE_MAX_CMD_LENGTH];
	sprintf(realCommand, m_commandStack.top().data, m_seq.next());
	
#ifdef ARDRONE_DEBUG
	std::cout << "Sending " << realCommand << std::endl;
#endif
	
	if(m_atSocket.sendto(realCommand, strlen(realCommand), m_atAddress) < 0) {
		perror("DroneController::run -> sendto");
		return false;
	}
	
	return true;
}

bool DroneController::setupSocket(Socket &socket, const unsigned short bindTo) const
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

std::map<std::string, std::string> DroneController::configuration(const double timeout)
{
	const double connectStart = seconds();
	while(isNotTalking() && seconds() - connectStart < timeout) msleep(100);
	if(isNotTalking()) return std::map<std::string, std::string>();
	
	Socket s = Socket::tcp();
	
	if(!s.setReusable(true)) return std::map<std::string, std::string>();
	if(!s.connect(m_configAddress)) return std::map<std::string, std::string>();
	if(!s.setBlocking(false)) return std::map<std::string, std::string>();
	
	control(5);
	control(4);
	
	char buffer[65536];
	buffer[0] = 0;
	const double start = seconds();
	
	ssize_t size = 0;
	do {
		size = s.recv(buffer, sizeof(buffer));
		msleep(100);
	} while(size < 0 && seconds() - start < timeout);
	
	s.close();
	
	if(size < 0) return std::map<std::string, std::string>();
	
	// std::cout << buffer << std::endl;
	
	std::map<std::string, std::string> ret;
	
	// We convert all delimeters to '\0', then walk through with basic C
	// string functions
	for(size_t i = 0; i < size; ++i) {
		if(buffer[i] == '\n') buffer[i] = 0;
		if(buffer[i] == '=') buffer[i - 1] = 0;
	}
	
	const char *walker = buffer;
	while(walker - buffer < size) {
		const char *const key = walker;
		const char *const value = walker + strlen(key) + 3;
		if(strlen(key) >= 1 && strlen(value) >= 1) ret[key] = value;
		walker = value + strlen(value) + 1;
	}
	
	return ret;
}

ARDrone::Version DroneController::findVersion()
{
	std::map<std::string, std::string> config = configuration();
	std::map<std::string, std::string>::const_iterator it = config.find("general:num_version_soft");
	if(it == config.end() || it->second.size() < 1) return ARDrone::Unknown;
	if(it->second[0] == '2') return ARDrone::V2;
	if(it->second[0] == '1') return ARDrone::V1;
	return ARDrone::Unknown;
}

void DroneController::setVideoFrameRate(const unsigned videoFrameRate)
{
	configure(ARDRONE_VIDEO_CODEC_FPS, videoFrameRate);
}

ARDrone::~ARDrone()
{
	disconnect();
}
	
bool ARDrone::connect(const char *const ip, const double timeout)
{
	m_controller->stop();
	m_controller->join();
	m_controller->start();
	
	/* if(!m_controller->restartProgramElf(Address(ip, 23))) {
		std::cout << "Warning: Failed to restart program.elf. Video might not work." << std::endl;
		return false;
	} else msleep(3000); */
	
	m_controller->setAtAddress(Address(ip, ARDRONE_AT_PORT));
	m_controller->setNavdataAddress(Address(ip, ARDRONE_NAVDATA_PORT));
	m_controller->setConfigAddress(Address(ip, ARDRONE_CONFIG_PORT));
	// This must be last right now
	m_controller->setVideoAddress(Address(ip, ARDRONE_VIDEO_PORT));
	
	if(!m_controller->isValid()) return false;
	
	m_controller->sendMagic();
	m_controller->setNavdataDemo(true);
	m_controller->setActiveCamera(m_activeCamera);
	// m_controller->control();
	
	// We know we're connected once we start
	// receiving navdata back
	double startWait = seconds();
	while(seconds() - startWait < timeout) {
		if(!m_controller->isNotTalking()) break;
	}
	/* if(m_controller->isNotTalking()) {
		m_controller->invalidate();
		return false;
	} */
	
	// Set ourself as the owner
	// m_controller->setSsid("Braden's Drone");
	
	m_controller->setVideoFrameRate(20);
	
	m_emergencyStop->stop();
	m_emergencyStop->join();
	m_emergencyStop->start();
	
	return true;
}

void ARDrone::disconnect()
{
	m_controller->land(true);
	msleep(100);
	while(state() != ARDrone::Landed && !m_controller->isNotTalking()) msleep(100);
	
	m_controller->invalidate();
	
	m_emergencyStop->stop();
	m_emergencyStop->join();
	
	m_controller->stop();
	m_controller->join();
}

ARDrone::Version ARDrone::version() const
{
	std::map<std::string, std::string> config = configuration();
	std::map<std::string, std::string>::const_iterator it = config.find("general:num_version_soft");
	if(it == config.end() || it->second.size() < 1) return ARDrone::Unknown;
	if(it->second[0] == '2') return ARDrone::V2;
	if(it->second[0] == '1') return ARDrone::V1;
	return ARDrone::Unknown;
}

ARDrone::NavigationData ARDrone::navigationData() const
{
	NavigationData ret;
	const navdata_demo_t *const demo = m_controller->latestNavdataDemo();
	if(!demo) return ret;
	
	ret.battery = demo->vbat_flying_percentage;
	
	ret.pitch = demo->pitch / 1000.0f;
	ret.roll = demo->roll / 1000.0f;
	ret.yaw = demo->yaw / 1000.0f;
	ret.altitude = demo->altitude / 1000.0f;
	
	ret.velocity.x = demo->velocity._0;
	ret.velocity.y = demo->velocity._1;
	ret.velocity.z = demo->velocity._2;
	
	ret.position = m_controller->position();
	
	return ret;
}

void ARDrone::clearPosition()
{
	m_controller->clearPosition();
}

void ARDrone::setSsid(const char *const ssid)
{
	m_controller->setSsid(ssid);
}

void ARDrone::setActiveCamera(const Camera activeCamera)
{
	m_activeCamera = activeCamera;
	m_controller->setActiveCamera(m_activeCamera);
}

ARDrone::Camera ARDrone::activeCamera() const
{
	return m_activeCamera;
}

void ARDrone::flatTrim()
{
	m_controller->flatTrim();
}

void ARDrone::takeoff()
{
	m_controller->takeoff();
}

void ARDrone::land()
{
	m_controller->land();
}

void ARDrone::setEmergencyStopEnabled(const bool emergencyStopEnabled)
{
	m_emergencyStop->setEnabled(emergencyStopEnabled);
}

bool ARDrone::isEmergencyStopEnabled() const
{
	return m_emergencyStop->isEnabled();
}

void ARDrone::hover()
{
	m_controller->move(0.0f, 0.0f, 0.0f, 0.0f);
}

void ARDrone::move(const float x, const float y, const float z, const float yaw)
{
	m_controller->move(x, y, z, yaw);
}

std::map<std::string, std::string> ARDrone::configuration() const
{
	return m_controller->configuration();
}

void ARDrone::rawImage(cv::Mat &image) const
{
	m_controller->image(image);
}

ARDrone::State ARDrone::state() const
{
	if(m_controller->isNotTalking()) return ARDrone::Disconnected;
	
	const navdata_t *const navdata = m_controller->latestNavdata();
	if(!navdata) return ARDrone::Landed;
	if(navdata->ardrone_state & ARDRONE_FLY_MASK) return ARDrone::Flying;
	return ARDrone::Landed;
}

void ARDrone::setOwnerAddress(const char *const address)
{
	m_controller->setOwner(address);
}

void ARDrone::pair()
{
	const char *const addr = Private::NetworkInterface::macAddress("wlan0");
	if(!addr) return;
	printf("setting owner as %s\n", addr);
	setOwnerAddress(addr);
	delete[] addr;
}

ARDrone *ARDrone::instance()
{
	static ARDrone s_instance;
	return &s_instance;
}

ARDrone::ARDrone()
	: m_controller(new DroneController),
	m_emergencyStop(new Private::ARDroneEmergencyStop(this)),
	m_activeCamera(ARDrone::Off)
{
}

ARDrone::ARDrone(const ARDrone &)
{
	
}

ARDrone &ARDrone::operator =(const ARDrone &rhs)
{
	return *this;
}

Camera::ARDroneInputProvider::ARDroneInputProvider()
	: m_opened(false)
{
}

bool Camera::ARDroneInputProvider::open(const int number)
{
	if(ARDrone::instance()->state() == ARDrone::Disconnected) {
		std::cout << "Failed to open the ARDrone's camera (disconnected)" << std::endl;
		return false;
	}
	
	ARDrone::instance()->setActiveCamera(number == 0
		? ARDrone::Front : ARDrone::Bottom);
	
	m_opened = true;
	return true;
}

bool Camera::ARDroneInputProvider::isOpen() const
{
	return m_opened;
}

void Camera::ARDroneInputProvider::setWidth(const unsigned width)
{
	// NYI
}

void Camera::ARDroneInputProvider::setHeight(const unsigned height)
{
	// NYI
}

bool Camera::ARDroneInputProvider::next(cv::Mat &image)
{
	ARDrone *const drone = ARDrone::instance();
	if(!m_opened || drone->state() == ARDrone::Disconnected)
		return false;
	
	drone->rawImage(image);
	return true;
}

bool Camera::ARDroneInputProvider::close()
{
	if(!m_opened) return false;
	ARDrone::instance()->setActiveCamera(ARDrone::Off);
	m_opened = false;
	return true;
}

#endif

/*
 * ardrone.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_ARDRONE_HPP_
#define INCLUDE_WALLABY_ARDRONE_HPP_

#ifndef _WIN32

#include <opencv2/core/core.hpp>
#include <map>
#include <string>

#include "camera.hpp"
#include "types.hpp"

class DroneController;

namespace Private
{
	class ARDroneEmergencyStop;
}

class EXPORT_SYM ARDrone
{
public:
	enum State
	{
		Disconnected = 0,
		Landed,
		Flying
	};

	enum Camera
	{
    Off = 0,
		Front,
		Bottom
	};

	enum Version
	{
		Unknown = 0,
		V1,
		V2
	};

	struct NavigationData
	{
		uint32_t battery;

		float pitch;
		float roll;
		float yaw;

		float altitude;

		Vec3f velocity;
		Vec3f position;
	};

	~ARDrone();


	bool connect(const char *const ip = "192.168.1.1", const double timeout = 3.0);

	/**
	 * Disconnects from the AR.Drone. The AR.Drone will perform an emergency landing (violently falling).
	 */
	void disconnect();

	/**
	 * Gets the version of the connected AR.Drone
	 */
	Version version() const;

	NavigationData navigationData() const;

	void clearPosition();

	/**
	 * Sets the SSID of the drone. The drone must be rebooted for this
	 * change to take effect.
	 * \param ssid The new ssid
	 */
	void setSsid(const char *const ssid);

	void setActiveCamera(const Camera activeCamera);
	Camera activeCamera() const;

	void flatTrim();
	void takeoff();
	void land();

	void setEmergencyStopEnabled(const bool emergencyStopEnabled);
	bool isEmergencyStopEnabled() const;

	void setOwnerAddress(const char *const address);
	void pair();

	/**
	 * Tells the AR.Drone to hover. This is equivalent to move(0.0f, 0.0f, 0.0f, 0.0f)
	 * \see move
	 */
	void hover();

	void move(const float x, const float y, const float z, const float yaw);

	std::map<std::string, std::string> configuration() const;

	void rawImage(cv::Mat &image) const;

	ARDrone::State state() const;

	static ARDrone *instance();

private:
	ARDrone();
	ARDrone(const ARDrone &);

	ARDrone &operator =(const ARDrone &rhs);

	DroneController *m_controller;
	Private::ARDroneEmergencyStop *m_emergencyStop;
	Camera m_activeCamera;
};

namespace Camera
{
	class EXPORT_SYM ARDroneInputProvider : public InputProvider
	{
	public:
		ARDroneInputProvider();
		virtual bool open(const int number);
		virtual bool isOpen() const;
		virtual void setWidth(const unsigned width);
		virtual void setHeight(const unsigned height);
		virtual bool next(cv::Mat &image);
		virtual bool close();

	private:
		bool m_opened;
	};
}

#endif


#endif /* INCLUDE_WALLABY_ARDRONE_HPP_ */

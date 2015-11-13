#ifndef _ARDRONE_CONSTANTS_P_HPP_
#define _ARDRONE_CONSTANTS_P_HPP_

// General
#define ARDRONE_CMD_SEPARATOR ((char)10)
#define ARDRONE_MAX_CMD_LENGTH (1024)

// Ports
#define ARDRONE_NAVDATA_PORT (5554)
#define ARDRONE_VIDEO_PORT (5555)
#define ARDRONE_AT_PORT (5556)
#define ARDRONE_CONFIG_PORT (5559)

// "AT" commands
#define ARDRONE_AT_PREFIX "AT*"
#define ARDRONE_AT_SUFFIX "="

#define ARDRONE_AT_REF (ARDRONE_AT_PREFIX "REF" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_PCMD (ARDRONE_AT_PREFIX "PCMD" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_FTRIM (ARDRONE_AT_PREFIX "FTRIM" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_CONFIG (ARDRONE_AT_PREFIX "CONFIG" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_CONFIG_IDS (ARDRONE_AT_PREFIX "CONFIG_IDS" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_LED (ARDRONE_AT_PREFIX "LED" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_ANIM (ARDRONE_AT_PREFIX "ANIM" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_COMWDG (ARDRONE_AT_PREFIX "COMWDG" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_CTRL (ARDRONE_AT_PREFIX "CTRL" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_PMODE (ARDRONE_AT_PREFIX "PMODE" ARDRONE_AT_SUFFIX)
#define ARDRONE_AT_MISC (ARDRONE_AT_PREFIX "MISC" ARDRONE_AT_SUFFIX)

// Config
#define ARDRONE_TRUE ("TRUE")
#define ARDRONE_FALSE ("FALSE")

#define ARDRONE_CFG_GET_CONTROL_MODE (4)
#define ARDRONE_ACK_CONTROL_MODE (5)

#define ARDRONE_NAVDATA_DEMO ("general:navdata_demo")
#define ARDRONE_NETWORK_OWNER_MAC ("network:owner_mac")
#define ARDRONE_NETWORK_SSID ("network:ssid_single_player")
#define ARDRONE_VIDEO_CODEC ("video:video_codec")
#define ARDRONE_VIDEO_CHANNEL ("video:video_channel")
#define ARDRONE_VIDEO_CODEC_FPS ("video:codec_fps")

// Navdata
#define ARDRONE_NAVDATA_HEADER (0x55667788)

#define ARDRONE_NAVDATA_DEMO_TAG (0)
#define ARDRONE_NAVDATA_TIME_TAG (1 << 1)
#define ARDRONE_NAVDATA_RAW_MEASURES_TAG (1 << 2)
#define ARDRONE_NAVDATA_PHYS_MEASURES_TAG (1 << 3)
#define ARDRONE_NAVDATA_GYROS_OFFSETS_TAG (1 << 4)
#define ARDRONE_NAVDATA_EULER_ANGLES_TAG (1 << 5)
#define ARDRONE_NAVDATA_REFERENCES_TAG (6)
#define ARDRONE_NAVDATA_TRIMS_TAG (7)
#define ARDRONE_NAVDATA_RC_REFERENCES_TAG (8)
#define ARDRONE_NAVDATA_PWM_TAG (9)
#define ARDRONE_NAVDATA_ALTITUDE_TAG (10)
#define ARDRONE_NAVDATA_VISION_RAW_TAG (11)
#define ARDRONE_NAVDATA_VISION_OF_TAG (12)
#define ARDRONE_NAVDATA_VISION_TAG (13)
#define ARDRONE_NAVDATA_VISION_PERF_TAG (14)
#define ARDRONE_NAVDATA_TRACKERS_SEND_TAG (15)
#define ARDRONE_NAVDATA_VISION_DETECT_TAG (16)
#define ARDRONE_NAVDATA_WATCHDOG_TAG (17)
#define ARDRONE_NAVDATA_ADC_DATA_FRAME_TAG (18)
#define ARDRONE_NAVDATA_VIDEO_STREAM_TAG (19)
#define ARDRONE_NAVDATA_GAMES_TAG (20)
#define ARDRONE_NAVDATA_PRESSURE_RAW_TAG (21)
#define ARDRONE_NAVDATA_MAGNETO_TAG (22)
#define ARDRONE_NAVDATA_WIND_TAG (23)
#define ARDRONE_NAVDATA_KALMAN_PRESSURE_TAG (24)
#define ARDRONE_NAVDATA_HDVIDEO_STREAM_TAG (25)
#define ARDRONE_NAVDATA_WIFI_TAG (26)
#define ARDRONE_NAVDATA_CKS_TAG (0xFFFF)

typedef enum {
	ARDRONE_FLY_MASK            = 1U << 0,  /*!< FLY MASK : (0) ardrone is landed, (1) ardrone is flying */
	ARDRONE_VIDEO_MASK          = 1U << 1,  /*!< VIDEO MASK : (0) video disable, (1) video enable */
	ARDRONE_VISION_MASK         = 1U << 2,  /*!< VISION MASK : (0) vision disable, (1) vision enable */
	ARDRONE_CONTROL_MASK        = 1U << 3,  /*!< CONTROL ALGO : (0) euler angles control, (1) angular speed control */
	ARDRONE_ALTITUDE_MASK       = 1U << 4,  /*!< ALTITUDE CONTROL ALGO : (0) altitude control inactive (1) altitude control active */
	ARDRONE_USER_FEEDBACK_START = 1U << 5,  /*!< USER feedback : Start button state */
	ARDRONE_COMMAND_MASK        = 1U << 6,  /*!< Control command ACK : (0) None, (1) one received */
	ARDRONE_CAMERA_MASK         = 1U << 7,  /*!< CAMERA MASK : (0) camera not ready, (1) Camera ready */
	ARDRONE_TRAVELLING_MASK     = 1U << 8,  /*!< Travelling mask : (0) disable, (1) enable */
	ARDRONE_USB_MASK            = 1U << 9,  /*!< USB key : (0) usb key not ready, (1) usb key ready */
	ARDRONE_NAVDATA_DEMO_MASK   = 1U << 10, /*!< Navdata demo : (0) All navdata, (1) only navdata demo */
	ARDRONE_NAVDATA_BOOTSTRAP   = 1U << 11, /*!< Navdata bootstrap : (0) options sent in all or demo mode, (1) no navdata options sent */
	ARDRONE_MOTORS_MASK         = 1U << 12, /*!< Motors status : (0) Ok, (1) Motors problem */
	ARDRONE_COM_LOST_MASK       = 1U << 13, /*!< Communication Lost : (1) com problem, (0) Com is ok */
	ARDRONE_SOFTWARE_FAULT      = 1U << 14, /*!< Software fault detected - user should land as quick as possible (1) */
	ARDRONE_VBAT_LOW            = 1U << 15, /*!< VBat low : (1) too low, (0) Ok */
	ARDRONE_USER_EL             = 1U << 16, /*!< User Emergency Landing : (1) User EL is ON, (0) User EL is OFF*/
	ARDRONE_TIMER_ELAPSED       = 1U << 17, /*!< Timer elapsed : (1) elapsed, (0) not elapsed */
	ARDRONE_MAGNETO_NEEDS_CALIB = 1U << 18, /*!< Magnetometer calibration state : (0) Ok, no calibration needed, (1) not ok, calibration needed */
	ARDRONE_ANGLES_OUT_OF_RANGE = 1U << 19, /*!< Angles : (0) Ok, (1) out of range */
	ARDRONE_WIND_MASK           = 1U << 20, /*!< WIND MASK: (0) ok, (1) Too much wind */
	ARDRONE_ULTRASOUND_MASK     = 1U << 21, /*!< Ultrasonic sensor : (0) Ok, (1) deaf */
	ARDRONE_CUTOUT_MASK         = 1U << 22, /*!< Cutout system detection : (0) Not detected, (1) detected */
	ARDRONE_PIC_VERSION_MASK    = 1U << 23, /*!< PIC Version number OK : (0) a bad version number, (1) version number is OK */
	ARDRONE_ATCODEC_THREAD_ON   = 1U << 24, /*!< ATCodec thread ON : (0) thread OFF (1) thread ON */
	ARDRONE_NAVDATA_THREAD_ON   = 1U << 25, /*!< Navdata thread ON : (0) thread OFF (1) thread ON */
	ARDRONE_VIDEO_THREAD_ON     = 1U << 26, /*!< Video thread ON : (0) thread OFF (1) thread ON */
	ARDRONE_ACQ_THREAD_ON       = 1U << 27, /*!< Acquisition thread ON : (0) thread OFF (1) thread ON */
	ARDRONE_CTRL_WATCHDOG_MASK  = 1U << 28, /*!< CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled */
	ARDRONE_ADC_WATCHDOG_MASK   = 1U << 29, /*!< ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good */
	ARDRONE_COM_WATCHDOG_MASK   = 1U << 30, /*!< Communication Watchdog : (1) com problem, (0) Com is ok */
#if defined(__ARMCC_VERSION)
	ARDRONE_EMERGENCY_MASK      = (int)0x80000000  /*!< Emergency landing : (0) no emergency, (1) emergency */
#else
	ARDRONE_EMERGENCY_MASK      = 1U << 31  /*!< Emergency landing : (0) no emergency, (1) emergency */
#endif
} def_ardrone_state_mask_t;

// Video
#define ARDRONE_VIDEO_NULL_CODEC (0x00)
#define ARDRONE_VIDEO_UVLC_CODEC (0x20)
#define ARDRONE_VIDEO_P264_CODEC (0x40)
#define ARDRONE_VIDEO_MP4_360P_CODEC (0x80)
#define ARDRONE_VIDEO_H264_360P_CODEC (0x81)
#define ARDRONE_VIDEO_H264_720P_CODEC (0x83)

// Used in conjunction with video:video_channel
#define ARDRONE_FRONT_CAMERA (0)
#define ARDRONE_BOTTOM_CAMERA (1)

// Application IDs
#define ARDRONE_APPLICATION_ID ("d87f7e0c")
#define ARDRONE_USER_ID ("be27e2e4")
#define ARDRONE_SESSION_ID ("d2e081a3")

#endif

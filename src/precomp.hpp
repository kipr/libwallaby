//
// Taken and modified from opencv v4.0 to support cap_v4l.cpp
//
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __VIDEOIO_H_
#define __VIDEOIO_H_

//#include "opencv2/videoio.hpp"
//#include "opencv2/videoio/legacy/constants_c.h"

//#include "opencv2/core/utility.hpp"
//#include "opencv2/core/private.hpp"

//#include <opencv2/core/utils/configuration.private.hpp>
//#include <opencv2/core/utils/logger.hpp>

//#include "opencv2/imgcodecs.hpp"

//#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"

//#include "opencv2/videoio/videoio_c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>  // FIXIT remove this

#if defined _WIN32 || defined WINCE
    #if !defined _WIN32_WINNT
        #ifdef HAVE_MSMF
            #define _WIN32_WINNT 0x0600 // Windows Vista
        #else
            #define _WIN32_WINNT 0x0501 // Windows XP
        #endif
    #endif

    #include <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#define __BEGIN__ __CV_BEGIN__
#define __END__  __CV_END__
#define EXIT __CV_EXIT__

/***************************** CvCapture structure ******************************/

struct CvCapture
{
    virtual ~CvCapture() {}
    virtual double getProperty(int) const { return 0; }
    virtual bool setProperty(int, double) { return 0; }
    virtual bool grabFrame() { return true; }
    virtual IplImage* retrieveFrame(int) { return 0; }
    virtual int getCaptureDomain() { return 0; } // Return the type of the capture object: CAP_DSHOW, etc...
};

/*************************** CvVideoWriter structure ****************************/

struct CvVideoWriter
{
    virtual ~CvVideoWriter() {}
    virtual bool writeFrame(const IplImage*) { return false; }
    virtual int getCaptureDomain() const { return 0; } // Return the type of the capture object: CAP_FFMPEG, etc...
};

CvCapture * cvCreateCameraCapture_V4L( int index );
CvCapture * cvCreateCameraCapture_V4L( const char* deviceName );
CvCapture * cvCreateCameraCapture_DC1394_2( int index );
CvCapture* cvCreateCameraCapture_MIL( int index );
CvCapture* cvCreateCameraCapture_Giganetix( int index );
CvCapture* cvCreateFileCapture_Win32( const char* filename );
CvVideoWriter* cvCreateVideoWriter_Win32( const char* filename, int fourcc,
                                          double fps, CvSize frameSize, int is_color );
CvCapture* cvCreateCameraCapture_DShow( int index );
CvCapture* cvCreateCameraCapture_OpenNI( int index );
CvCapture* cvCreateCameraCapture_OpenNI2( int index );
CvCapture* cvCreateFileCapture_OpenNI( const char* filename );
CvCapture* cvCreateFileCapture_OpenNI2( const char* filename );
CvCapture* cvCreateCameraCapture_Android( int index );
CvCapture* cvCreateCameraCapture_XIMEA( int index );
CvCapture* cvCreateCameraCapture_XIMEA( const char* serialNumber );
CvCapture* cvCreateCameraCapture_AVFoundation(int index);
CvCapture* cvCreateCameraCapture_Aravis( int index );

CvVideoWriter* cvCreateVideoWriter_Images(const char* filename);


#define CV_CAP_GSTREAMER_1394		0
#define CV_CAP_GSTREAMER_V4L		1
#define CV_CAP_GSTREAMER_V4L2		2
#define CV_CAP_GSTREAMER_FILE		3

CvCapture * cvCreateFileCapture_QT (const char  * filename);
CvCapture * cvCreateCameraCapture_QT  (const int     index);

CvVideoWriter* cvCreateVideoWriter_QT ( const char* filename, int fourcc,
                                        double fps, CvSize frameSize, int is_color );

CvCapture* cvCreateFileCapture_AVFoundation (const char * filename);
CvVideoWriter* cvCreateVideoWriter_AVFoundation( const char* filename, int fourcc,
                                                double fps, CvSize frameSize, int is_color );


CvCapture * cvCreateCameraCapture_PvAPI  (const int     index);
CvVideoWriter* cvCreateVideoWriter_GStreamer( const char* filename, int fourcc,
                                            double fps, CvSize frameSize, int is_color );


namespace cv
{

//#`fdef CVCAPANY
/// from videoio.hpp

/** @brief %VideoCapture API backends identifier.
 *
 * Select preferred API for a capture object.
 *e used in the VideoCapture::VideoCapture() constructor or VideoCapture::open()
 *
 * @note Backends are available only if they have been built with your OpenCV binaries.
 * See @ref videoio_overview for more information.
 * */
//#ifdef DELETETHIS	
    enum VideoCaptureAPIs {
 	    CAP_ANY          = 0,            //!< Auto detect == 0
	    CAP_VFW          = 200,          //!< Video For Windows (obsolete, removed)
        CAP_V4L          = 200,          //!< V4L/V4L2 capturing support
        CAP_V4L2         = CAP_V4L,      //!< Same as CAP_V4L
        CAP_FIREWIRE     = 300,          //!< IEEE 1394 drivers
        CAP_FIREWARE     = CAP_FIREWIRE, //!< Same value as CAP_FIREWIRE
        CAP_IEEE1394     = CAP_FIREWIRE, //!< Same value as CAP_FIREWIRE
        CAP_DC1394       = CAP_FIREWIRE, //!< Same value as CAP_FIREWIRE
        CAP_CMU1394      = CAP_FIREWIRE, //!< Same value as CAP_FIREWIRE
        CAP_QT           = 500,          //!< QuickTime (obsolete, removed)
        CAP_UNICAP       = 600,          //!< Unicap drivers (obsolete, removed)
        CAP_DSHOW        = 700,          //!< DirectShow (via videoInputdf)
        CAP_PVAPI        = 800,          //!< PvAPI, Prosilica GigE SDK
        CAP_OPENNI       = 900,          //!< OpenNI (for Kinect)
        CAP_OPENNI_ASUS  = 910,          //!< OpenNI (for Asus Xtion)
        CAP_ANDROID      = 1000,         //!< Android - not used
        CAP_XIAPI        = 1100,         //!< XIMEA Camera API
        CAP_AVFOUNDATION = 1200,         //!< AVFoundation framework for iOS (OS X Lion will have the same API)
        CAP_GIGANETIX    = 1300,         //!< Smartek Giganetix GigEVisionSDK
        CAP_MSMF         = 1400,         //!< Microsoft Media Foundation (via videoInput)
        CAP_WINRT        = 1410,         //!< Microsoft Windows Runtime using Media Foundation
        CAP_INTELPERC    = 1500,         //!< Intel Perceptual Computing SDK
        CAP_OPENNI2      = 1600,         //!< OpenNI2 (for Kinect)
        CAP_OPENNI2_ASUS = 1610,         //!< OpenNI2 (for Asus Xtion and Occipital Structure sensors)
        CAP_GPHOTO2      = 1700,         //!< gPhoto2 connection
        CAP_GSTREAMER    = 1800,         //!< GStreamer
        CAP_FFMPEG       = 1900,         //!< Open and record video file or stream using the FFMPEG library
        CAP_IMAGES       = 2000,         //!< OpenCV Image Sequence (e.g. img_%02d.jpg)
        CAP_ARAVIS       = 2100,         //!< Aravis SDK
        CAP_OPENCV_MJPEG = 2200,         //!< Built-in OpenCV MotionJPEG codecv
        CAP_INTEL_MFX    = 2300,         //!< Intel MediaSDK
        CAP_XINE         = 2400,         //!< XINE engine (Linux)
     };
    
    enum VideoCaptureProperties {
        CAP_PROP_POS_MSEC       =0, //!< Current position of the video file in milliseconds.
        CAP_PROP_POS_FRAMES     =1, //!< 0-based index of the frame to be decoded/captured next.
        CAP_PROP_POS_AVI_RATIO  =2, //!< Relative position of the video file: 0=start of the film, 1=end of the film.
        CAP_PROP_FRAME_WIDTH    =3, //!< Width of the frames in the video stream.
        CAP_PROP_FRAME_HEIGHT   =4, //!< Height of the frames in the video stream.
        CAP_PROP_FPS            =5, //!< Frame rate.
        CAP_PROP_FOURCC         =6, //!< 4-character code of codec. see VideoWriter::fourcc .
        CAP_PROP_FRAME_COUNT    =7, //!< Number of frames in the video file.
        CAP_PROP_FORMAT         =8, //!< Format of the %Mat objects returned by VideoCapture::retrieve().
        CAP_PROP_MODE           =9, //!< Backend-specific value indicating the current capture mode.
        CAP_PROP_BRIGHTNESS    =10, //!< Brightness of the image (only for those cameras that support).
        CAP_PROP_CONTRAST      =11, //!< Contrast of the image (only for cameras).
        CAP_PROP_SATURATION    =12, //!< Saturation of the image (only for cameras).
        CAP_PROP_HUE           =13, //!< Hue of the image (only for cameras).
        CAP_PROP_GAIN          =14, //!< Gain of the image (only for those cameras that support).
        CAP_PROP_EXPOSURE      =15, //!< Exposure (only for those cameras that support).
        CAP_PROP_CONVERT_RGB   =16, //!< Boolean flags indicating whether images should be converted to RGB.
        CAP_PROP_WHITE_BALANCE_BLUE_U =17, //!< Currently unsupported.
        CAP_PROP_RECTIFICATION =18, //!< Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently).
        CAP_PROP_MONOCHROME    =19,
        CAP_PROP_SHARPNESS     =20,
        CAP_PROP_AUTO_EXPOSURE =21, //!< DC1394: exposure control done by camera, user can adjust reference level using this feature.
        CAP_PROP_GAMMA         =22,
        CAP_PROP_TEMPERATURE   =23,
        CAP_PROP_TRIGGER       =24,
        CAP_PROP_TRIGGER_DELAY =25,
        CAP_PROP_WHITE_BALANCE_RED_V =26,
        CAP_PROP_ZOOM          =27,
        CAP_PROP_FOCUS         =28,
        CAP_PROP_GUID          =29,
        CAP_PROP_ISO_SPEED     =30,
        CAP_PROP_BACKLIGHT     =32,
        CAP_PROP_PAN           =33,
        CAP_PROP_TILT          =34,
        CAP_PROP_ROLL          =35,
        CAP_PROP_IRIS          =36,
        CAP_PROP_SETTINGS      =37, //!< Pop up video/camera filter dialog (note: only supported by DSHOW backend currently. The property value is ignored)
        CAP_PROP_BUFFERSIZE    =38,
        CAP_PROP_AUTOFOCUS     =39,
        CAP_PROP_SAR_NUM       =40, //!< Sample aspect ratio: num/den (num)
        CAP_PROP_SAR_DEN       =41, //!< Sample aspect ratio: num/den (den)
        CAP_PROP_BACKEND       =42, //!< Current backend (enum VideoCaptureAPIs). Read-only property
        CAP_PROP_CHANNEL       =43, //!< Video input or Channel Number (only for those cameras that support)
        CAP_PROP_AUTO_WB       =44, //!< enable/ disable auto white-balance
        CAP_PROP_WB_TEMPERATURE=45, //!< white-balance color temperature
#ifndef CV_DOXYGEN
        CV__CAP_PROP_LATEST
#endif
    };

    class IVideoCapture
    {
    public:
        virtual ~IVideoCapture() {}
        virtual double getProperty(int) const { return 0; }
        virtual bool setProperty(int, double) { return false; }
        virtual bool grabFrame() = 0;
        virtual bool retrieveFrame(int, OutputArray) = 0;
        virtual bool isOpened() const = 0;
        virtual int getCaptureDomain() { return 0; } // Return the type of the capture object: CAP_DSHOW, etc...
    };

    class IVideoWriter
    {
    public:
        virtual ~IVideoWriter() {}
        virtual double getProperty(int) const { return 0; }
        virtual bool setProperty(int, double) { return false; }

        virtual bool isOpened() const = 0;
        virtual void write(InputArray) = 0;

        virtual int getCaptureDomain() const { return cv::CAP_ANY; } // Return the type of the capture object: CAP_FFMPEG, etc...
    };

    Ptr<IVideoCapture> createFileCapture_Images(const String& filename);

    Ptr<IVideoCapture> createMotionJpegCapture(const String& filename);
    Ptr<IVideoWriter> createMotionJpegWriter(const String& filename, int fourcc, double fps, Size frameSize, bool iscolor);

    Ptr<IVideoCapture> createGPhoto2Capture(int index);
    Ptr<IVideoCapture> createGPhoto2Capture(const String& deviceName);


    Ptr<IVideoCapture> createXINECapture(const char* filename);

    Ptr<IVideoCapture> createGStreamerCapture(const String& filename);
    Ptr<IVideoCapture> createGStreamerCapture(int index);

    Ptr<cv::IVideoCapture> cvCreateFileCapture_FFMPEG_proxy(const String& filename);
    Ptr<IVideoWriter> cvCreateVideoWriter_FFMPEG_proxy(const String& filename, int fourcc, double fps, Size frameSize, int isColor);

    Ptr<IVideoCapture> cvCreateCapture_MSMF(int index);
    Ptr<IVideoCapture> cvCreateCapture_MSMF(const String& filename);
    Ptr<IVideoWriter> cvCreateVideoWriter_MSMF(const String& filename, int fourcc, double fps, Size frameSize, int is_color);
}

#endif /* __VIDEOIO_H_ */

/*!
 * \file camera.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup camera Camera
 */
/**
 * \page Camera
 * The functions here deal with the camera.
 * Specifically, the functions here deal with
 * object detection through the use of color.
 * \section setup Setup
 * To use the object detections, you'll first have
 * to have a `.conf` file. On a wombat, you can go to 
 * `settings`, then `channels`, then add a configuration.
 * Next, edit that configuration and add a new channel
 * (for the color object detection, it should be HSV Blob Tracking).
 * Finally, configure that channel, selecting the color that
 * you want the camera to detect when detecting objects on that
 * channel.
 * \section example_object_detection Example object detection
 * Assuming you have a configuration file "botball.conf" and
 * you have a Channel 0 on that file configured to detect red
 * pom poms, then the following code would print 'red' when
 * a red pom is seen.
 * ```
 * #include <kipr/wombat.h>
 * #include <stdio.h>
 * 
 * int main(){
 *      camera_load_config("botball.conf");
 *      camera_open();
 *      
 *      // wait a little and update the camera
 *      int i = 0;
 *      for (i = 0; i < 10; i++){
 *          camera_update();
 *          msleep(10);
 *      }
 * 
 *      int red_object_seen = 0;
 *      while (!red_object_seen){
 *          // get the bounding box of the largest object in
 *          // channel 0 (the red channel)
 *          rectangle object_bounding_box = get_object_bbox(0, 0);
 *          if (object_bounding_box.width * object_bounding_box.height != 0){
 *              printf("red object seen!");
 *              red_object_seen = 1;
 *          }
 * 
 *          // update the camera
 *          camera_update();
 *          msleep(10);
 *      }
 * 
 *      camera_close();  // cleanup the camera
 *      return 0;
 * }
 * ```
 * \ingroup camera
 */

#ifndef _KIPR_CAMERA_CAMERA_H_
#define _KIPR_CAMERA_CAMERA_H_

#include "kipr/geometry/geometry.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief A struct that represents a pixel in an image
 * 
 */
typedef struct pixel
{
  int r;  //!< the red value of the pixel
  int g;  //!< the green value of the pixel
  int b;  //!< the blue value of the pixel
} pixel;

enum Resolution
{
  LOW_RES,
  MED_RES,
  HIGH_RES,
  TELLO_RES,
  NATIVE_RES
};

enum Model
{
  WHITE_2016,
  BLACK_2017,
  TELLO
};

/**
 * Opens the default system camera for use at LOW_RES (160x120).
 * \return 1 on success, 0 on failure
 * \see camera_open_at_res
 * \see camera_open_device
 * \see camera_close
 * \ingroup camera
 */
int camera_open();

/**
 * Opens the default system camera for use at LOW_RES (160x120).
 * This will improve frame rates for the black Logitech camera
 * \return 1 on success, 0 on failure
 * \see camera_open_at_res
 * \see camera_open_device
 * \see camera_close
 * \ingroup camera
 */
int camera_open_black();

/**
 * Opens the default system camera for use at a given resolution.
 * \param res The resolution the camera should operate at. This can be:
 *   - LOW_RES (160x120)
 *   - MED_RES (320x240)
 *   - HIGH_RES (640x480)
 *   - TELLO_RES (1280x720)
 * \warning Only LOW_RES is currently supported. The function will fail for other resolutions.
 * \return 1 on success, 0 on failure
 * \see camera_open
 * \see camera_open_device
 * \see camera_close
 * \ingroup camera
 */
int camera_open_at_res(enum Resolution res);

/**
 * Opens a camera for use.
 * \param number The camera's id. 0 is the first camera, 1 is the second camera, etc.
 * \param res The resolution the camera should operate at. This can be:
 *   - LOW_RES (160x120)
 *   - MED_RES (320x240)
 *   - HIGH_RES (640x480)
 *   - TELLO_RES (1280x720)
 * \warning Only LOW_RES is currently supported. The function will fail for other resolutions.
 * \return 1 on success, 0 on failure
 * \see camera_open
 * \see camera_close
 * \ingroup camera
 */
int camera_open_device(int number, enum Resolution res);

/**
 * Opens a camera for use.
 * \param number The camera's id. 0 is the first camera, 1 is the second camera, etc.
 * \param res The resolution the camera should operate at. This can be:
 *   - LOW_RES (160x120)
 *   - MED_RES (320x240)
 *   - HIGH_RES (640x480)
 *   - TELLO_RES (1280x720)
 * \param model The camera model
 *   - WHITE_2016  The white 2016 Botball camera
 *   - BLACK_2017  The black 2017 Botball camera
 *   - TELLO       Tello drone camera
 * \warning MED_RES is supported only for the BLACK_2017 camera and HIGH_RES is not supported
 * \return 1 on success, 0 on failure
 * \see camera_open
 * \see camera_close
 * \ingroup camera
 */
int camera_open_device_model_at_res(int number, enum Model model, enum Resolution res);

/**
 * Loads the config file specified by name. The system will look for the config in the base path.
 * \param name The configuration to load. Configuration file names are case sensitive.
 * \note Do NOT include the config file extension ".conf" in the name parameter.
 * \return 1 on success, 0 on failure.
 * \see set_camera_config_base_path
 * \ingroup camera
 */
int camera_load_config(const char *name);

/**
 * Gets the camera's x resolution.
 * \attention This value might be different than the previously set x resolution. Never assume the x resolution.
 * \return The camera's x resolution, in pixels.
 * \ingroup camera
 */
int get_camera_width(void);

/**
 * Gets the camera's y resolution.
 * \attention This value might be different than the previously set y resolution. Never assume the y resolution.
 * \return The camera's y resolution, in pixels.
 * \ingroup camera
 */
int get_camera_height(void);

/**
 * Pulls a new image from the camera for processing.
 * \return 1 on success, 0 on failure.
 * \ingroup camera
 */
int camera_update(void);

/**
 * Gets the color of a pixel.
 * \param p The point at which the pixel lies.
 * \return The rgb value of the pixel located at point p.
 * \note A (r, g, b) value of (-1, -1, -1) will be returned for points that are out of range.
 * \ingroup camera
 */
pixel get_camera_pixel(point2 p);

/**
 * \return Number of channels in the current configuration.
 * \see get_object_count
 * \ingroup camera
 */
int get_channel_count(void);

/**
 * \param channel The channel to scan for objects.
 * \note Objects are sorted by area, largest first.
 * \return Number of objects in the given channel, -1 if channel doesn't exist.
 * \see get_channel_count
 * \ingroup camera
 */
int get_object_count(int channel);

/**
 * \return The string data associated with a given object on a given channel.
 * If there is no data associated, 0 is returned.
 * \note This data is not guaranteed to be null terminated.
 * \note This string pointer will be invalid after a call to camera_update()
 * \see get_object_data_length
 * \ingroup camera
 */
const char *get_object_data(int channel, int object);

/**
 * \return The data associated with the given channel and object as an integer. If the given channel or object doesn't exist, -1 is returned.
 * \see get_object_data
 */
int get_code_num(int channel, int object);

/**
 * \return The length of the string data associated with a given object on a given channel.
 * If there is no data associated, 0 is returned. If the channel or object is invalid, 0 is returned.
 * \see get_object_data
 * \ingroup camera
 */
int get_object_data_length(int channel, int object);

/**
 * \return The confidence, between 0.0 and 1.0, that given object on the given channel is significant.
 * If the channel or object doesn't exist, 0.0 is returned.
 * \ingroup camera
 */
double get_object_confidence(int channel, int object);

/**
 * \return The object's bounding box area. -1 is returned if the channel or object doesn't exist.
 * \ingroup camera
 */
int get_object_area(int channel, int object);

/**
 * \return The bounding box of the given object on the given channel.
 * \ingroup camera
 */
rectangle get_object_bbox(int channel, int object);
int get_object_bbox_ulx(int channel, int object);
int get_object_bbox_uly(int channel, int object);
int get_object_bbox_brx(int channel, int object);
int get_object_bbox_bry(int channel, int object);
int get_object_bbox_width(int channel, int object);
int get_object_bbox_height(int channel, int object);

/**
 * \return The (x, y) centroid of the given object on the given channel.
 * \ingroup camera
 */
point2 get_object_centroid(int channel, int object);
int get_object_centroid_column(int channel, int object);
int get_object_centroid_x(int channel, int object);
int get_object_centroid_row(int channel, int object);
int get_object_centroid_y(int channel, int object);

/**
 * \return The (x, y) center of the given object on the given channel.
 * \ingroup camera
 */
point2 get_object_center(int channel, int object);
int get_object_center_column(int channel, int object);
int get_object_center_x(int channel, int object);
int get_object_center_row(int channel, int object);
int get_object_center_y(int channel, int object);

/**
 * Cleanup the current camera instance.
 * \see camera_open
 * \see camera_open_at_res
 * \see camera_open_device
 * \ingroup camera
 */
void camera_close();

/**
 * Sets the path in which to look for camera configurations.
 * \param path the absolute directory path in which to look for camera configurations.
 * \ingroup camera
 */
void set_camera_config_base_path(const char *const path);

/**
 * Retrieves the current camera frame row as a BGR (BGR888) array. The returned
 * pointer is invalid after camera_update() is called again.
 *
 * \return the current BGR888 camera frame row.
 * \ingroup camera
 */
const unsigned char *get_camera_frame_row(unsigned row);

/**
 * Retrieves the current camera frame as a BGR (BGR888) array. The returned
 * pointer is invalid after camera_update() is called again.
 *
 * \return the current BGR888 camera frame.
 * \ingroup camera
 */
const unsigned char *get_camera_frame();

unsigned get_camera_element_size();

#ifdef __cplusplus
}
#endif

#endif

/*
 * camera.h
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "geom.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pixel
{
	int r;
	int g;
	int b;
} pixel;

enum Resolution
{
	LOW_RES,
	MED_RES,
	HIGH_RES,
	NATIVE_RES
};

int camera_open();

int camera_open_at_res(enum Resolution res);

int camera_open_device(int number, enum Resolution res);

int camera_load_config(const char *name);

void set_camera_width(int width);

void set_camera_height(int height);

int get_camera_width(void);

int get_camera_height(void);

int camera_update(void);

pixel get_camera_pixel(point2 p);

int get_channel_count(void);

int get_object_count(int channel);

const char *get_object_data(int channel, int object);

int get_code_num(int channel, int object);

int get_object_data_length(int channel, int object);

double get_object_confidence(int channel, int object);

int get_object_area(int channel, int object);

rectangle get_object_bbox(int channel, int object);
int get_object_bbox_ulx(int channel, int object);
int get_object_bbox_uly(int channel, int object);
int get_object_bbox_brx(int channel, int object);
int get_object_bbox_bry(int channel, int object);
int get_object_bbox_width(int channel, int object);
int get_object_bbox_height(int channel, int object);

point2 get_object_centroid(int channel, int object);
int get_object_centroid_column(int channel, int object);
int get_object_centroid_x(int channel, int object);
int get_object_centroid_row(int channel, int object);
int get_object_centroid_y(int channel, int object);

point2 get_object_center(int channel, int object);
int get_object_center_column(int channel, int object);
int get_object_center_x(int channel, int object);
int get_object_center_row(int channel, int object);
int get_object_center_y(int channel, int object);

void camera_close();

void set_camera_config_base_path(const char *const path);

const unsigned char *get_camera_frame_row(unsigned row);

const unsigned char *get_camera_frame();

unsigned get_camera_element_size();


#ifdef __cplusplus
}
#endif


#endif
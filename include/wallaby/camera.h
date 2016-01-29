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

int camera_update(void);

pixel get_camera_pixel(point2 p);

void camera_close();

const unsigned char *get_camera_frame_row(unsigned row);

const unsigned char *get_camera_frame();


#ifdef __cplusplus
}
#endif


#endif
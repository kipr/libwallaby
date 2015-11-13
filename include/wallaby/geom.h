/*
 * geom.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_GEOM_H_
#define INCLUDE_WALLABY_GEOM_H_


#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct point2
{
	int x;
	int y;
} point2;

typedef struct point3
{
	int x;
	int y;
	int z;
} point3;

typedef struct rectangle
{
	int ulx;
	int uly;
	int width;
	int height;
} rectangle;

VF EXPORT_SYM point2 create_point2(int x, int y);
VF EXPORT_SYM point3 create_point3(int x, int y, int z);
VF EXPORT_SYM rectangle create_rectangle(int ulx, int uly, int width, int height);

VFL

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_GEOM_H_ */

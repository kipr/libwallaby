#ifndef _KIPR_GEOMETRY_GEOMETRY_H_
#define _KIPR_GEOMETRY_GEOMETRY_H_

#include "kipr/export/export.h"

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

EXPORT_SYM point2 create_point2(int x, int y);
EXPORT_SYM point3 create_point3(int x, int y, int z);
EXPORT_SYM rectangle create_rectangle(int ulx, int uly, int width, int height);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_GEOM_H_ */

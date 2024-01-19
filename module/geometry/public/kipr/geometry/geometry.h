#ifndef _KIPR_GEOMETRY_GEOMETRY_H_
#define _KIPR_GEOMETRY_GEOMETRY_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A 2D point; has x and y
 * \ingroup geometry
 */
typedef struct point2
{
  int x;  //!< the x value of the point
  int y;  //!< the y value of the point
} point2;

/**
 * \brief A 3D point; has x, y, and z
 * \ingroup geometry
 */
typedef struct point3
{
  int x;  //!< the x value of the point
  int y;  //!< the y value of the point
  int z;  //!< the z value of the point
} point3;

/**
 * \brief A rectangle object.
 * \ingroup geometry
 */
typedef struct rectangle
{
  int ulx;     //!< the x value of the rectangle's upper left
  int uly;     //!< the y value of the rectangle's upper left
  int width;   //!< the width of the rectangle
  int height;  //!< the height of the rectangle
} rectangle;

/**
 * \brief Create a point2 object
 * 
 * \param x - the x value of the point
 * \param y - the y value of the point
 * \returns point2 object
 * \ingroup geometry
 */
EXPORT_SYM point2 create_point2(int x, int y);

/**
 * \brief Create a point3 object
 * 
 * \param x - the x value of the point
 * \param y - the y value of the point
 * \param z - the z value of the point
 * \return point3 object 
 * \ingroup geometry
 */
EXPORT_SYM point3 create_point3(int x, int y, int z);

/**
 * \brief Create a rectangle object
 * 
 * \param ulx - the x value of the rectangle's upper left
 * \param uly - the y value of the rectangle's upper left
 * \param width - the width of the rectangle
 * \param height - the height of the rectangle
 * \return rectangle object
 * \ingroup geometry
 */
EXPORT_SYM rectangle create_rectangle(int ulx, int uly, int width, int height);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_GEOM_H_ */

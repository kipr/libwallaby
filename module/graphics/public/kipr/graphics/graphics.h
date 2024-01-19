/*!
 * \file graphics.h
 * \brief Methods for displaying graphics to the screen.
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup graphics Graphics
 */

#ifndef _KIPR_GRAPHICS_GRAPHICS_H_
#define _KIPR_GRAPHICS_GRAPHICS_H_

#include "kipr/export/export.h"

#include "graphics_key_code.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Opens a KISS Graphics Window for drawing and input functions.
 *
 * \param width The width of the graphics window
 * \param height The height of the graphics window
 * \ingroup graphics
 */
EXPORT_SYM int graphics_open(int width, int height);

/**
 * Closes the previously opened KISS Graphics Window
 * \ingroup graphics
 */
EXPORT_SYM void graphics_close();

/**
 * Update must be called after drawing to the window for changes to be visible.
 * This function also must be called regularly for input to work.
 * \ingroup graphics
 */
EXPORT_SYM void graphics_update();

/**
 * Fills the entire window with the color black
 * \ingroup graphics
 */
EXPORT_SYM void graphics_clear();

/**
 * @brief An enumerate with encoding types RGB and BGR
 * \ingroup graphics
 */
typedef enum Encoding
{
  RGB,  //!< the enum used to represent RGB encoding
  BGR   //!< the enum used to represent BGR encoding
} Encoding;

/**
 * This function is equivalent to calling graphics_blit_enc() with an encoding of RGB
 * \see graphics_blit_enc
 */
EXPORT_SYM void graphics_blit(const unsigned char *data, int x, int y, int width, int height);

/**
 * This function is equivalent to calling graphics_blit_region_enc() with an encoding of RGB
 * \see graphics_blit_region_enc
 */
EXPORT_SYM void graphics_blit_region(const unsigned char *data, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy);

/**
 * Blits a pixel array onto the graphics context
 *
 * \param data the 1D array of pixel data to blit
 * \param enc RGB for a Red-Green-Blue encoding or BGR for a Blue-Green-Red encoding
 * \param x The destination x coordinate of the blit (target)
 * \param y The destination y coordinate of the blit (target)
 * \param width The width of the area to blit
 * \param height The height of the area to blit
 * \ingroup graphics
 */
EXPORT_SYM void graphics_blit_enc(const unsigned char *data, Encoding enc, int x, int y, int width, int height);

/**
 * Blits a pixel array region onto the graphics context
 *
 * \param data the 1D array of pixel data to blit
 * \param enc RGB for a Red-Green-Blue encoding or BGR for a Blue-Green-Red encoding
 * \param sx The start x coordinate of the blit (source)
 * \param sy The start y coordinate of the blit (source)
 * \param ex The end x coordinate of the blit (source)
 * \param ey The end y coordinate of the blit (source)
 * \param width The width of the area to blit
 * \param height The height of the area to blit
 * \param dx The destination x coordinate of the blit (target)
 * \param dy The destination y coordinate of the blit (target)
 * \ingroup graphics
 */
EXPORT_SYM void graphics_blit_region_enc(const unsigned char *data, Encoding enc, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy);

/**
 * Fill the graphics context with a color.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_fill(int r, int g, int b);

/**
 * Draw a pixel at (x, y)
 * \param x the X coordinate of the pixel
 * \param y the Y coordinate of the pixel
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_pixel(int x, int y, int r, int g, int b);

/**
 * Draw a line from the point (x1, y1) to (x2, y2)
 * \param x1 the X coordinate of the first point
 * \param y1 the Y coordinate of the first point
 * \param x2 the X coordinate of the second point
 * \param y2 the Y coordinate of the second point
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw an unfilled circle to the graphics window at the given coordinates and with the given radius.
 * \param cx the X coordinate of the center of the circle
 * \param cy the Y cordinate of the center of the circle
 * \param radius the radius of the circle to be drawn
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_circle(int cx, int cy, int radius, int r, int g, int b);

/**
 * Draw a filled circle to the graphics window at the given coordinates and with the given radius.
 * \param cx the X coordinate of the center of the circle
 * \param cy the Y cordinate of the center of the circle
 * \param radius the radius of the circle to be drawn
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b);

/**
 * Draw an unfilled rectangle to the graphics window at the given coordinates.
 * \param x1 the X coordinate of the rectangle's top-left corner
 * \param y1 the Y coordinate of the rectangle's top-left corner
 * \param x2 the X coordinate of the rectangle's bottom-right corner
 * \param y2 the Y coordinate of the rectangle's bottom-right corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw a filled rectangle to the graphics window at the given coordinates.
 * \param x1 the X coordinate of the rectangle's top-left corner
 * \param y1 the Y coordinate of the rectangle's top-left corner
 * \param x2 the X coordinate of the rectangle's bottom-right corner
 * \param y2 the Y coordinate of the rectangle's bottom-right corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw an unfilled triangle to the graphics window at the given coordinates.
 * The order of the corners does not matter when drawing a triangle.
 * \param x1 the X coordinate of the triangle's first corner
 * \param y1 the Y coordinate of the triangle's first corner
 * \param x2 the X coordinate of the triangle's second corner
 * \param y2 the Y coordinate of the triangle's second corner
 * \param x3 the X coordinate of the triangle's third corner
 * \param y3 the Y coordinate of the triangle's third corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);

/**
 * Draw a filled triangle to the graphics window at the given coordinates.
 * The order of the corners does not matter when drawing a triangle.
 * \param x1 the X coordinate of the triangle's first corner
 * \param y1 the Y coordinate of the triangle's first corner
 * \param x2 the X coordinate of the triangle's second corner
 * \param y2 the Y coordinate of the triangle's second corner
 * \param x3 the X coordinate of the triangle's third corner
 * \param y3 the Y coordinate of the triangle's third corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_triangle_fill(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);

/**
 * Gets the current state of a keyboard key.
 *
 * \note A KISS Graphics Window must be open for input functions to work.
 * \param key An ASCII key code or a Key value from the KeyCode enum.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_key_state(enum KeyCode key);

/**
 * Returns the mouse's current position relative to the top left corner of the graphics window.
 * \ingroup graphics
 */
EXPORT_SYM void get_mouse_position(int *x, int *y);

/**
 * Returns the state of the mouse's middle button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_middle_button();

/**
 * Returns the state of the mouse's left button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_left_button();

/**
 * Returns the state of the mouse's right button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_right_button();

#ifdef __cplusplus
}
#endif

#endif

/*!
* \file graphics_characters.h
* \brief functions for adding alphameric characters to graphics windows.

   These are functions for adding alphameric characters (caps only) to graphics windows.
   The functions were extracted from a graphing package written by D.Miller Oct 2008.
   Character generation capabilities were extended from 7-segment to 16-segment and
   enhanced to include basic punctuation characters, plus size and bold-face options
   by C.Winton Oct 2008.
* \copyright KISS Insitute for Practical Robotics
* \defgroup graphics Graphics
*/

#ifndef _KIPR_GRAPHICS_GRAPHICS_CHARACTERS_H_
#define _KIPR_GRAPHICS_GRAPHICS_CHARACTERS_H_

/*
Segment arrangement
	 --0- -1--     ---18----     --20 21--     ---- ----
	|\   |   /|	  |\  18   /|	|\20   21/|	  ||   |   /|
	7 8  9 10 2	  | \  |  / |	|24  |  26|	  ||\ 29  / |
	|   \|/   |	  |   17/   |	24   |/  26	  || 29|/   |
	 -15- -11-	   ---17----	 ---- ----	   |--- ----
	|   /|\   |	  |   /|\   |	25  /|\  27	  |2  /|\   |
	6 14 1 12 3	  |  / |  \ |	|25  |  27|	  |8 / |  \ |
	|/   3   \|	  |/  16   \|	|/22   23\|	  ||   |   \|
 	 --5- -4--	   ---16----	 --22 23--	   ---- ----
                     /
                	  / (19)
*/
//! The number of segments in a character display
#define NUMSEG 30
//! SEGL specifies the length of a vertical segment in pixels
#define SEGL 5
//! SEGSP is the spacing of characters
#define SEGSP 7

//! the variable used to store whether or not characters should be bold
extern int __bold;
//! BOLD means have bold text
#define BOLD __bold=1;
//! NOBOLD means do not have bold text
#define NOBOLD __bold=0;

//! The width in pixels of the window
#ifndef GXWINDOW
  #define GXWINDOW 700
#endif
//! The height of the window
#ifndef GYWINDOW
  #define GYWINDOW 500
#endif

// **************** Function Prototypes ******************
// Based on 16 segment characters: draws a segment for each s value that gets a 1 and a rectangle if the value is 2;
// the upper left corner starts at x,y; the color is defined by the vales of rgb; multiplier size resizes the character;
// there are added segments to provide for . : , ; and other non alphameric characters
// arrows (l,r,u,d) are specified by numbers 11 ,12, 13, 14

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Draw a 16 segment character (alphameric - caps only)
 *
 * \param s The segments to display
 * \param x The X coordinate of the first segment's top-left corner
 * \param y The Y coordinate of the first segment's top-left corner
 * \param r The red component of the drawing, from 0 to 255
 * \param g The green component of the drawing, from 0 to 255
 * \param b The blue component of the drawing, from 0 to 255
 * \param size The segment's size
 * \ingroup graphics
 */
void graphics_segment_display(int s[], int x, int y, int r, int g, int b, float size);


/**
 * Prints out a character in 16 segment characters
 *
 * \param n The character to display
 * \param x The X coordinate of the character's top-left corner
 * \param y The Y coordinate of the character's top-left corner
 * \param r The red component of the drawing, from 0 to 255
 * \param g The green component of the drawing, from 0 to 255
 * \param b The blue component of the drawing, from 0 to 255
 * \param size The character's size
 * \ingroup graphics
 */
void graphics_print_character(int n, int x, int y, int r, int g, int b, float size);


/**
 * Prints out a text string in 16 segment characters
 *
 * \param s The string to display
 * \param x The X coordinate of the first character's top-left corner
 * \param y The Y coordinate of the first character's top-left corner
 * \param r The red component of the drawing, from 0 to 255
 * \param g The green component of the drawing, from 0 to 255
 * \param b The blue component of the drawing, from 0 to 255
 * \param size The strings's size
 * \ingroup graphics
 */
void graphics_print_string(char s[], int x, int y, int r, int g, int b, float size);


/**
 * Prints out an integer in 16 segment characters
 *
 * \param n The integer to display
 * \param minNumDigits minNumDigits is normally 0, but can be larger if leading 0's are desired
 * \param x The X coordinate of the first character's top-left corner
 * \param y The Y coordinate of the first character's top-left corner
 * \param r The red component of the drawing, from 0 to 255
 * \param g The green component of the drawing, from 0 to 255
 * \param b The blue component of the drawing, from 0 to 255
 * \param size The strings's size
 * \ingroup graphics
 */
int graphics_print_int(int n, int minNumDigits, int x, int y, int r, int g, int b, float size);


/**
 * Prints out a float in 16 segment characters
 *
 * \param n The float to display
 * \param numDigits numDigits is the number of digits to right of decimal to be printed
 * \param x The X coordinate of the first character's top-left corner
 * \param y The Y coordinate of the first character's top-left corner
 * \param r The red component of the drawing, from 0 to 255
 * \param g The green component of the drawing, from 0 to 255
 * \param b The blue component of the drawing, from 0 to 255
 * \param size The strings's size
 * \ingroup graphics
 */
void graphics_print_float(float n, int numDigits, int x, int y, int r, int g, int b, float size);


#ifdef __cplusplus
}
#endif

#endif

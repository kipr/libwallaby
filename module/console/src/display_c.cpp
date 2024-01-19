// Copyright(c) KIPR, 2013
// Full screen management functions for the display window as implemented on the KIPR LINK controller
//   display_printf(int column, int row, <printf arguments>)
//
// Console screen display window size is 10 rows and limited to 42 columns, indexed from 0
//
// Initial version: 1/8/2013 - cnw
// Revisions: 2/2/2013 - cnw
//    1. switched variadic usage to vsprintf
//    2. fixed to work for both normal and extra button cases
// Revision:  2/4/2013 - cnw
//    maxw failing to prevent scroll bar display; simply needed a reduction by 1
// stdarg.h provides macros for accessing a function's argument list ... see K&R

#include "kipr/button/button.h"

// void console_clear(); (to be defined in kipr/botball.h)
#define __VBUTTONS 2 // set to 0 if A,B,C virtual buttons are not going to be implemented
// Copyright(c) KIPR, 2013
// Full screen management functions for the display window as implemented on the KIPR LINK controller
//   display_printf(int column, int row, <printf arguments>)
//
// Link Console screen display window size is 10 rows and limited to 42 columns, indexed from 0
// Wallaby console is 13 rows and limited to 57 columns
//
// Initial version: 1/8/2013 - cnw
// Revision: 2/2/2013 - cnw
//    1. switched variadic usage to vsprintf
//    2. fixed to work for both normal and extra button cases
// Revision:  2/4/2013 - cnw
//    maxw failing to prevent scroll bar display; simply needed a reduction by 1
// Revision: 2/7/2016 - cnw
//    reconfigured for Wallaby controller - virtual buttons not in at time, which may require a further adjustment
// stdarg.h provides macros for accessing a function's argument list ... see K&R
// Revision: 8/6/2019 - cnw
//    reconfigured for Wombat KRC controller - reduced line count and column count
//    to conform to Wombat KRC UI display

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <kipr/console/console.h>
#include <kipr/console/display.h>

#define _MAPy 11 // 9 rows when 3 buttons visible, loses 2 rows for each line of virtual buttons
#define _MAPx 43 // 41 columns is max fit for screen (array has 42 to accommodate \0)
char _display_map[_MAPy][_MAPx];
int _initialize_ = 1; // flag to signal need to clear display on first use

void display_clear()
{ // clears console and sets display map to all spaces
  int i, j;
  console_clear();
  for (i = 0; i < _MAPy; i++)
    for (j = 0; j < _MAPx; j++)
      _display_map[i][j] = ' '; //  initialize to spaces
  for (i = 0; i < _MAPy; i++)
    _display_map[i][_MAPx - 1] = '\0'; // make each row (except last) a string
}

// Usage: same as printf except the first two parameters specify
//    the (column, row) of the display where print is to begin.
// Excess print to a line is truncated.
void display_printf(int col, int row, const char *t, ...)
{                       //  variadic function
  va_list argp;         // variadic argument list pointer for vsprintf
  int i, maxw;          // maxw marks available room on row
  int rb6 = __VBUTTONS; // row adjustment (default is 2, if there are no buttons, change to 0)
  char *dp, ws[256];    // pointer into display map, working string for vsprintf

  va_start(argp, t); // t is last named argument in display_printf's function header;
  // Note: system macro va_start points argp to first variadic arg for vsprintf
  if (get_extra_buttons_visible())
    rb6 = 4;
  if (col >= _MAPx)
  {
    row = row + 1;
    col = 0;
  } // bad col so wrap to next line
  if (row >= _MAPy)
    row = _MAPy - 1; // bad row so (over) print on last line
  console_clear();   // ready the display
  if (_initialize_ == 1)
  {
    display_clear();
    _initialize_ = 0;
  }                             // clear map to spaces on first call
  dp = &_display_map[row][col]; // starting point for printf
  maxw = _MAPx - col - 1;       // space remaining on line
  vsprintf(ws, t, argp);        // same as sprintf except requires a pointer to argument list
  for (i = 0; i < strlen(ws); i++)
  { // insert formatted phrase in display map
    if (maxw == 0)
      break;     // if no more room get out of this
    *dp = ws[i]; // insert next character from working string
    dp++;
    maxw--;
  }
  va_end(argp); // clean up
  for (i = 0; i < _MAPy - rb6 - 1; i++)
    printf("%s\n", _display_map[i]); // refresh the display
  printf("%s", _display_map[i]);     // last line printed without new line
}

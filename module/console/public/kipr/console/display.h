// Full screen management functions for the display window as implemented on the KIPR LINK controller
//   display_printf(int column, int row, <printf arguments>)
//   display_clear()
// NOTE: the difference between display_clear and console_clear is that display_clear also clears the display map
// Display window size is 10 rows by 42 columns, indexed from 0

#ifndef _KIPR_CONSOLE_DISPLAY_H_
#define _KIPR_CONSOLE_DISPLAY_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

//void display_printf(int col, int row, char *t, ...);
//void display_clear();

EXPORT_SYM void display_clear();  // clears console and sets display map to all spaces

EXPORT_SYM void display_printf(int col, int row, const char *t, ...); // runs printf formatting to specified screen location

#ifdef __cplusplus
}
#endif

#endif

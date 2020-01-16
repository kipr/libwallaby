/**************************************************************************
 *  Copyright 2013 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

// Full screen management functions for the display window as implemented on the KIPR LINK controller
//   display_printf(int column, int row, <printf arguments>)
//   display_clear()
// NOTE: the difference between display_clear and console_clear is that display_clear also clears the display map
// Display window size is 10 rows by 42 columns, indexed from 0

#ifndef INCLUDE_WALLABY_DISPLAY_H_
#define INCLUDE_WALLABY_DISPLAY_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

//void display_printf(int col, int row, char *t, ...);
//void display_clear();

VF EXPORT_SYM void display_clear();  // clears console and sets display map to all spaces

EXPORT_SYM void display_printf(int col, int row, const char *t, ...); // runs printf formatting to specified screen location

VFL

#ifdef __cplusplus
}
#endif

#endif

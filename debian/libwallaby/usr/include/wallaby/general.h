/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
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

/*!
 * \file general.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \defgroup general General
 */

#ifndef _GENERAL_H_
#define _GENERAL_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Sets whether or not hardware output command publishing happens automatically.
 * \details publish() is, by default, called on your behalf by libkovan. When you turn automatic
 * publishing off, libkovan expects you to publish() changes manually.
 * \param[in] on 1 for automatic publishing, 0 for manual publishing
 * \note Any command involving hardware modification, such as motors or servos,
 * requires publish() to be called before changes will be written to the system.
 * \ingroup general
 * \deprecated This has no effect when using the Wallaby
 */
VF EXPORT_SYM void set_auto_publish(int on);

/*!
 * \brief Explictly publishes new data to the system
 * \details All hardware output commands are stored locally before being written to the system.
 * This allows for batch execution of commands with automatic publishing turned off.
 * \ingroup general
 * \deprecated This has no effect when using the Wallaby
 */
VF EXPORT_SYM void publish();

VF EXPORT_SYM void halt();

VFL

void freeze_halt();

#ifdef __cplusplus
}
#endif

#endif

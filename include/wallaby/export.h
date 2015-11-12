/*
 * export.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_EXPORT_H_
#define INCLUDE_WALLABY_EXPORT_H_

#ifdef WIN32
#define EXPORT_SYM __declspec(dllexport)
#else
#define EXPORT_SYM
#endif


#endif /* INCLUDE_WALLABY_EXPORT_H_ */

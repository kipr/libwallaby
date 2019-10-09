/*
 * util.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_UTIL_H_
#define INCLUDE_WALLABY_UTIL_H_


#ifdef __cplusplus
extern "C" {
#endif

void msleep(long msecs);

void iitxash(long msecs);

unsigned long systime();

double seconds();

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_UTIL_H_ */

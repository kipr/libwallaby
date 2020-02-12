/*
 * audio.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_AUDIO_H_
#define INCLUDE_WALLABY_AUDIO_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * Trigger an audible beep
 * \note does not seem to work on the Wallaby
 */
VF EXPORT_SYM void beep(void);

VFL

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_AUDIO_H_ */

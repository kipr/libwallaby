#ifndef _KIPR_AUDIO_AUDIO_H_
#define _KIPR_AUDIO_AUDIO_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Trigger an audible beep
 * \note does not seem to work on the Wallaby
 */
EXPORT_SYM void beep(void);

#ifdef __cplusplus
}
#endif

#endif

/*!
 * \file thread.h
 * \brief Threading methods
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup thread Threading
 */

#ifndef _KIPR_THREAD_THREAD_H_
#define _KIPR_THREAD_THREAD_H_

#include "kipr/export/export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void *data;
} mutex;

typedef struct
{
	void *data;
} thread;

typedef void (*thread_function)();

/**
 * \ingroup thread
 */
EXPORT_SYM mutex mutex_create();

/**
 * \ingroup thread
 */
EXPORT_SYM void mutex_lock(mutex m);

/**
 * \ingroup thread
 */
EXPORT_SYM int mutex_trylock(mutex m);

/**
 * \ingroup thread
 */
EXPORT_SYM void mutex_unlock(mutex m);

/**
 * \ingroup thread
 */
EXPORT_SYM void mutex_destroy(mutex m);

/**
 * \ingroup thread
 */
EXPORT_SYM thread thread_create(thread_function func);

/**
 * \ingroup thread
 */
EXPORT_SYM void thread_start(thread id);

/**
 * \ingroup thread
 */
EXPORT_SYM void thread_wait(thread id);

/**
 * \ingroup thread
 */
EXPORT_SYM void thread_destroy(thread id);

#ifdef __cplusplus
}
#endif

#endif
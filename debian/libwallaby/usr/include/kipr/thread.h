/*
 * thread.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_THREAD_H_
#define INCLUDE_WALLABY_THREAD_H_


/*!
 * \file thread.h
 * \brief Threading methods
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup thread Threading
 */

#include "export.h"
#include "vtable.h"

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
VF EXPORT_SYM mutex mutex_create();

/**
 * \ingroup thread
 */
VF EXPORT_SYM void mutex_lock(mutex m);

/**
 * \ingroup thread
 */
VF EXPORT_SYM int mutex_trylock(mutex m);

/**
 * \ingroup thread
 */
VF EXPORT_SYM void mutex_unlock(mutex m);

/**
 * \ingroup thread
 */
VF EXPORT_SYM void mutex_destroy(mutex m);

/**
 * \ingroup thread
 */
VF EXPORT_SYM thread thread_create(thread_function func);

/**
 * \ingroup thread
 */
VF EXPORT_SYM void thread_start(thread id);

/**
 * \ingroup thread
 */
VF EXPORT_SYM void thread_wait(thread id);

/**
 * \ingroup thread
 */
VF EXPORT_SYM void thread_destroy(thread id);

VFL

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_THREAD_H_ */

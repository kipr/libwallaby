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
extern "C"
{
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
	 * Create a mutex.
	 * \note mutexes are used to prevent memory errors when two programs or threads need
	 * to access the same variable.
	 * \ingroup thread
	 */
	VF EXPORT_SYM mutex mutex_create();

	/**
	 * Locks the mutex. This means that only the thread that locked the mutex
	 * will run until the mutex is unlocked.
	 * \see mutex_unlock
	 * \ingroup thread
	 */
	VF EXPORT_SYM void mutex_lock(mutex m);

	/**
	 * Try to lock the mutex.
	 * \return 1 on success locking, 0 on failure to lock.
	 * \see mutex_lock
	 * \ingroup thread
	 */
	VF EXPORT_SYM int mutex_trylock(mutex m);

	/**
	 * Unlocks the mutex. This means that other threads (if present) will run
	 * again.
	 * \see mutex_lock
	 * \ingroup thread
	 */
	VF EXPORT_SYM void mutex_unlock(mutex m);

	/**
	 * Destroy the mutex. This is used for cleanup after your program
	 * has finished with the mutex.
	 * \ingroup thread
	 */
	VF EXPORT_SYM void mutex_destroy(mutex m);

	/**
	 * Create a thread that will run the given function.
	 * \param func The function that the thread will run
	 * \ingroup thread
	 */
	VF EXPORT_SYM thread thread_create(thread_function func);

	/**
	 * Run the given thread
	 * \param id The thread to run
	 * \ingroup thread
	 */
	VF EXPORT_SYM void thread_start(thread id);

	/**
	 * Wait until the given thread has finished running.
	 * \param id The thread to wait for.
	 * \ingroup thread
	 */
	VF EXPORT_SYM void thread_wait(thread id);

	/**
	 * Destroy the thread. This is used for cleanup after your
	 * program has finished with the thread.
	 * \ingroup thread
	 */
	VF EXPORT_SYM void thread_destroy(thread id);

	VFL

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WALLABY_THREAD_H_ */

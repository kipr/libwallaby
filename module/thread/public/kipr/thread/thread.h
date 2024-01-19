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

/**
 * \brief A mutex, or lock
 * \details mutexes are used to prevent memory errors when two programs or threads need
 * to access the same variable.
 * \see mutex_create
 * \ingroup thread
 */
typedef struct
{
	void *data;  //!< a C++ Mutex object cast to a `void*`. Not intended to be accessed.
} mutex;

/**
 * \brief A thread, or separate process 
 * \details a thread runs a function "in the background"
 * (while another function is executing).
 * \see thread_create
 * \ingroup thread
 */
typedef struct
{
	void *data;  //!< a C++ Thread object cast to a `void*`. Not intended to be accessed.
} thread;

/** \brief thread_function is a wrapper for functions of all return types.
 * \details thread_function is defined so that
 * any function will be accepted by thread_start
 * by automatically casting it to a thread_function
 * so functions with all return types, whether int, float, etc, can
 * be threaded
 * \ingroup thread
 */
typedef void (*thread_function)();

/**
 * Create a mutex.
 * \see mutex
 * \ingroup thread
 */
EXPORT_SYM mutex mutex_create();

/**
 * Locks the mutex. This means that only the thread that locked the mutex
 * will run until the mutex is unlocked.
 * \see mutex_unlock
 * \ingroup thread
 */
EXPORT_SYM void mutex_lock(mutex m);

/**
 * Try to lock the mutex.
 * \return 1 on success locking, 0 on failure to lock.
 * \see mutex_lock
 * \ingroup thread
 */
EXPORT_SYM int mutex_trylock(mutex m);

/**
 * Unlocks the mutex. This means that other threads (if present) will run
 * again.
 * \see mutex_lock
 * \ingroup thread
 */
EXPORT_SYM void mutex_unlock(mutex m);

/**
 * Destroy the mutex. This is used for cleanup after your program
 * has finished with the mutex.
 * \ingroup thread
 */
EXPORT_SYM void mutex_destroy(mutex m);

/**
 * Create a thread that will run the given function.
 * \param func The function that the thread will run
 * \ingroup thread
 */
EXPORT_SYM thread thread_create(thread_function func);

/**
 * Run the given thread
 * \param id The thread to run
 * \ingroup thread
 */
EXPORT_SYM void thread_start(thread id);

/**
 * Wait until the given thread has finished running.
 * \param id The thread to wait for.
 * \ingroup thread
 */
EXPORT_SYM void thread_wait(thread id);

/**
 * Destroy the thread. This is used for cleanup after your
 * program has finished with the thread.
 * \ingroup thread
 */
EXPORT_SYM void thread_destroy(thread id);

#ifdef __cplusplus
}
#endif

#endif
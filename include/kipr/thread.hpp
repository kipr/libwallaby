/*
 * thread.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_THREAD_HPP_
#define INCLUDE_WALLABY_THREAD_HPP_


#ifndef WIN32
#include <pthread.h>
#else
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "export.h"

class EXPORT_SYM Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	bool tryLock();

	void unlock();

private:
	Mutex(const Mutex &rhs);

#ifdef WIN32
	CRITICAL_SECTION m_handle;
#else
	pthread_mutex_t m_handle;
#endif
};

class EXPORT_SYM Thread
{
public:
	Thread();
	virtual ~Thread();

	void start();
	void join();

	virtual void run() = 0;

private:
#ifndef WIN32
	pthread_t m_thread;
#else
	HANDLE m_thread;
#endif
};


#endif /* INCLUDE_WALLABY_THREAD_HPP_ */

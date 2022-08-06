#include "kipr/thread/thread.h"
#include "kipr/thread/thread.hpp"

using namespace kipr;
using namespace kipr::thread;

class FunctionThread : public Thread
{
public:
	FunctionThread(thread_function func)
		: m_func(func)
	{
	}
	
	void run()
	{
		(*m_func)();
	}
	
private:
	thread_function m_func;
};

Mutex *mutexObject(void *data)
{
	return reinterpret_cast<Mutex *>(data);
}

FunctionThread *threadObject(void *data)
{
	return reinterpret_cast<FunctionThread *>(data);
}

mutex mutexStruct(Mutex *m)
{
	mutex ret;
	ret.data = reinterpret_cast<void *>(m);
	return ret;
}

::thread threadStruct(FunctionThread *t)
{
	::thread ret;
	ret.data = reinterpret_cast<void *>(t);
	return ret;
}

mutex mutex_create(void)
{
	return mutexStruct(new Mutex());
}

void mutex_lock(mutex m)
{
	mutexObject(m.data)->lock();
}

int mutex_trylock(mutex m)
{
	return mutexObject(m.data)->tryLock() ? 1 : 0;
}

void mutex_unlock(mutex m)
{
	mutexObject(m.data)->unlock();
}

void mutex_destroy(mutex m)
{
	delete mutexObject(m.data);
}

::thread thread_create(thread_function func)
{
	return threadStruct(new FunctionThread(func));
}

void thread_start(::thread id)
{
	threadObject(id.data)->start();
}

void thread_wait(::thread id)
{
	threadObject(id.data)->join();
}

void thread_destroy(::thread id)
{
	delete threadObject(id.data);
}

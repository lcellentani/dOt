#ifndef __DOT_MUTEX_H_HEADER_GUARD__
#define __DOT_MUTEX_H_HEADER_GUARD__

#if DOT_PLATFORM_ANDROID || DOT_PLATFORM_OSX
#	include <pthread.h>
#elif DOT_PLATFORM_WINDOWS
#	include <errno.h>
#	include <windows.h>
#	include <limits.h>
#endif // DOT_PLATFORM_

namespace dot
{
namespace core
{

class Mutex
{
public:
	Mutex()
	{
#if DOT_PLATFORM_POSIX
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mHandle, &attr);
#else
		InitializeCriticalSection(&mHandle);
#endif
	}

	~Mutex()
	{
#if DOT_PLATFORM_POSIX
		pthread_mutex_destroy(&mHandle);
#else
		DeleteCriticalSection(&mHandle);
#endif
	}

	void lock()
	{
#if DOT_PLATFORM_POSIX
		pthread_mutex_lock(&mHandle);
#else
		EnterCriticalSection(&mHandle);
#endif
	}

	void unlock()
	{
#if DOT_PLATFORM_POSIX
		pthread_mutex_unlock(&mHandle);
#else
		LeaveCriticalSection(&mHandle);
#endif
	}

private:
#if DOT_PLATFORM_POSIX
	pthread_mutex_t mHandle;
#else
	CRITICAL_SECTION mHandle;
#endif
};

class MutexScope
{
public:
	MutexScope(Mutex& mutex) : mMutex(mutex)
	{
		mMutex.lock();
	}

	~MutexScope()
	{
		mMutex.unlock();
	}

private:
	Mutex& mMutex;
};

} // namespace core
} // namespace dot

#endif // __DOT_MUTEX_H_HEADER_GUARD__

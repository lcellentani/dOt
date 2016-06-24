#ifndef __DOT_SEMAPHORE_H_HEADER_GUARD__
#define __DOT_SEMAPHORE_H_HEADER_GUARD__

#include "core/Mutex.h"

#if DOT_PLATFORM_POSIX
#	include <errno.h>
#	include <semaphore.h>
#	include <time.h>
#	include <pthread.h>
#elif DOT_PLATFORM_WINDOWS
#	include <windows.h>
#	include <limits.h>
#	include <stdint.h>
#endif // DOT_PLATFORM_

namespace dot
{
namespace core
{

#if DOT_PLATFORM_POSIX

	class Semaphore
	{
	public:
		Semaphore() : mCount(0)
		{
			int result;
			result = pthread_mutex_init(&mMutex, NULL);
			
			result = pthread_cond_init(&mCondition, NULL);
		}

		~Semaphore()
		{
			int result;
			result = pthread_cond_destroy(&mCondition);

			result = pthread_mutex_destroy(&mMutex);
		}

		void signal(uint32_t _count = 1)
		{
			int result = pthread_mutex_lock(&mMutex);

			for (uint32_t ii = 0; ii < _count; ++ii)
			{
				result = pthread_cond_signal(&mCondition);
			}

			mCount += _count;

			result = pthread_mutex_unlock(&mMutex);
		}

		bool wait(int32_t msecs = -1)
		{
			int result = pthread_mutex_lock(&mMutex);

#		if DOT_PLATFORM_OSX
			while (0 == result && 0 >= mCount)
			{
				result = pthread_cond_wait(&mCondition, &mMutex);
			}
#		elif DOT_PLATFORM_IOS
			if (-1 == _msecs)
			{
				while (0 == result && 0 >= mCount)
				{
					result = pthread_cond_wait(&mCondition, &mMutex);
				}
			}
			else
			{
				timespec ts;
				ts.tv_sec = _msecs / 1000;
				ts.tv_nsec = (_msecs % 1000) * 1000;

				while (0 == result && 0 >= mCount)
				{
					result = pthread_cond_timedwait_relative_np(&mCondition, &mMutex, &ts);
				}
			}
#		else
			timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += _msecs / 1000;
			ts.tv_nsec += (_msecs % 1000) * 1000;

			while (0 == result && 0 >= mCount)
			{
				result = pthread_cond_timedwait(&mCondition, &mMutex, &ts);
			}
#		endif
			bool ok = (0 == result);

			if (ok)
			{
				--mCount;
			}

			result = pthread_mutex_unlock(&mMutex);

			return ok;
		}

	private:
		pthread_mutex_t mMutex;
		pthread_cond_t mCondition;
		int32_t mCount;
	};

#elif DOT_PLATFORM_WINDOWS

	class Semaphore
	{
	public:
		Semaphore()
		{
			mHandle = CreateSemaphoreA(NULL, 0, LONG_MAX, NULL);
		}

		~Semaphore()
		{
			CloseHandle(mHandle);
		}

		void signal(uint32_t count = 1) const
		{
			ReleaseSemaphore(mHandle, count, NULL);
		}

		bool wait(int32_t msecs = -1) const
		{
			DWORD milliseconds = (0 > msecs) ? INFINITE : msecs;
			return WAIT_OBJECT_0 == WaitForSingleObject(mHandle, milliseconds);
		}

	private:
		HANDLE mHandle;
	};

#endif

} // namespace core
} // namespace dot

#endif // __DOT_SEMAPHORE_H_HEADER_GUARD__
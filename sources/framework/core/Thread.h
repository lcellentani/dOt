#ifndef __DOT_THREAD_H_HEADER_GUARD__
#define __DOT_THREAD_H_HEADER_GUARD__

#include "core/Semaphore.h"

#if DOT_PLATFORM_POSIX
#	include <pthread.h>
#elif DOT_PLATFORM_WINDOWS
#	include <windows.h>
#	include <limits.h>
#	include <stdint.h>
#endif

namespace dot
{
namespace core
{

typedef int32_t (*ThreadFunc)(void *userData);

class Thread
{
public:
	Thread()
#if DOT_PLATFORM_WINDOWS
		: mHandle(INVALID_HANDLE_VALUE)
		, mThreadId(UINT32_MAX)
#elif DOT_PLATFORM_POSIX
		: mHandle(0)
#endif
		, mWorkingFunc(nullptr)
		, mUserData(nullptr)
		, mStackSize(0)
		, mSemaphore()
		, mExitCode(0)
		, mRunning(false)
	{

	}

	virtual ~Thread()
	{
		if (mRunning)
		{
			Shutdown();
		}
	}

	void Start(ThreadFunc func, void *userData, uint32_t stackSize = 0, const char *name = nullptr)
	{
		mWorkingFunc = func;
		mUserData = userData;
		mStackSize = stackSize;
		mRunning = true;

#if DOT_PLATFORM_WINDOWS
		mHandle = ::CreateThread(NULL, mStackSize, (LPTHREAD_START_ROUTINE)threadFunc, this, 0, NULL);
#elif DOT_PLATFORM_POSIX
		int result = 0;

		pthread_attr_t attr;
		result = pthread_attr_init(&attr);

		if (0 != mStackSize)
		{
			result = pthread_attr_setstacksize(&attr, mStackSize);
		}

		result = pthread_create(&mHandle, &attr, &threadFunc, this);
#endif

		mSemaphore.wait();

		if (name != nullptr)
		{
			SetThreadName(name);
		}
	}

	void Shutdown()
	{
		if (mRunning)
		{
#if DOT_PLATFORM_WINDOWS
			WaitForSingleObject(mHandle, INFINITE);
			GetExitCodeThread(mHandle, (DWORD*)&mExitCode);
			CloseHandle(mHandle);
			mHandle = INVALID_HANDLE_VALUE;
#elif DOT_PLATFORM_POSIX
			union
			{
				void* ptr;
				int32_t i;
			} cast;
			pthread_join(mHandle, &cast.ptr);
			mExitCode = cast.i;
			mHandle = 0;
#endif
			mRunning = false;
		}
	}

	bool IsRunning() const
	{
		return mRunning;
	}

	int32_t GetExitCode() const
	{
		return mExitCode;
	}

	void SetThreadName(const char* name)
	{
#if DOT_PLATFORM_POSIX
		pthread_setname_np(name);
#elif DOT_PLATFORM_WINDOWS && DOT_COMPILER_MSVC
#	pragma pack(push, 8)
		const DWORD MS_VC_EXCEPTION = 0x406D1388;
		struct ThreadNameInfo
		{
			DWORD dwType;
			LPCSTR szName;
			DWORD dwThreadID;
			DWORD dwFlags;
		};
#	pragma pack(pop)
		ThreadNameInfo threadInfo;
		threadInfo.dwType = 0x1000;
		threadInfo.szName = name;
		threadInfo.dwThreadID = mThreadId;
		threadInfo.dwFlags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(threadInfo) / sizeof(ULONG_PTR), (ULONG_PTR*)&threadInfo);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
#endif
	}

private:
	int32_t ProxyWorker()
	{
#if DOT_PLATFORM_WINDOWS
		mThreadId = ::GetCurrentThreadId();
#endif

		mSemaphore.signal();

		return mWorkingFunc(mUserData);
	}

#if DOT_PLATFORM_WINDOWS
	static DWORD WINAPI threadFunc(LPVOID _arg)
	{
		Thread* thread = (Thread*)_arg;
		int32_t result = thread->ProxyWorker();
		return result;
	}
#else
	static void* threadFunc(void* arg)
	{
		Thread* thread = (Thread*)arg;
		union
		{
			void* ptr;
			int32_t i;
		} cast;
		cast.i = thread->ProxyWorker();
		return cast.ptr;
	}
#endif


#if DOT_PLATFORM_WINDOWS
	HANDLE mHandle;
	DWORD  mThreadId;
#elif DOT_PLATFORM_POSIX
	pthread_t mHandle;
#endif

	ThreadFunc mWorkingFunc;
	void *mUserData;
	uint32_t mStackSize;

	Semaphore mSemaphore;

	uint32_t mExitCode;
	bool mRunning;
};

} // namespace core
} // namespace dot

#endif // __DOT_THREAD_H_HEADER_GUARD__
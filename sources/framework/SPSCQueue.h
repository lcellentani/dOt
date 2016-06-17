#ifndef __DOT_SPSCQUEUE_H_HEADER_GUARD__
#define __DOT_SPSCQUEUE_H_HEADER_GUARD__

#include "Atomic.h"
#include "Mutex.h"
#include "Semaphore.h"

#include <stdint.h> // int32_t

#include <list>

namespace dot
{
namespace core
{

// http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=1
template<typename T>
class LockFreeQueue
{
public:
	LockFreeQueue() : mFirst(new Node(nullptr)), mDivider(mFirst), mLast(mFirst)
	{
	}

	~LockFreeQueue()
	{
		while (nullptr != mFirst)
		{
			Node *node = mFirst;
			if (node != nullptr)
			{
				mFirst = node->mNext;
				delete node;
			}
		}
	}

	void enqueue(T *ptr) // producer only
	{
		mLast->mNext = new Node((void *)ptr);
		AtomicExchangePtr((void **)&mLast, mLast->mNext);
		while (mFirst != mDivider)
		{
			Node *node = mFirst;
			mFirst = mFirst->mNext;
			delete node;
		}
	}

	T * dequeue() // consume only
	{
		if (mDivider != mLast)
		{
			T *ptr = (T *)mDivider->mNext->mPtr;
			AtomicExchangePtr((void **)&mDivider, mDivider->mNext);
			return ptr;
		}
		return nullptr;
	}

private:
	struct Node
	{
		Node(void *ptr) : mPtr(ptr), mNext(nullptr) {}

		void *mPtr;
		Node *mNext;
	};

	Node *mFirst;
	Node *mDivider;
	Node *mLast;
};

template<typename T>
class LockFreeQueueMutex
{
public:
	LockFreeQueueMutex() {}

	~LockFreeQueueMutex() {}

	void enqueue(T* item)
	{
		MutexScope lock(mMutex);
		mQueue.push_back(item);
	}

	T * dequeue()
	{
		MutexScope lock(mMutex);
		if (!mQueue.empty())
		{
			T *item = mQueue.front();
			mQueue.pop_front();
			return item;
		}
		return nullptr;
	}

private:
	Mutex mMutex;
	std::list<T> mQueue;
};

template<typename T>
class WaitFreeQueue
{
public:
	WaitFreeQueue() {}

	~WaitFreeQueue() {}

	void enqueue(T *ptr) // producer only
	{
		mQueue.enqueue((void *)ptr);
		mCount.signal();
	}

	T * dequeue(int32_t msecs = -1) // consumer only
	{
		if (mCount.wait(msecs))
		{
			return (T *)mQueue.dequeue();
		}
		return nullptr;
	}

private:
	Semaphore mCount;
	LockFreeQueue<void> mQueue;
};

} // namespace core
} // namespace dot

#endif // __DOT_SPSCQUEUE_H_HEADER_GUARD__
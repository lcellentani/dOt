#ifndef __DOT_MAINPROXY_H_HEADER_GUARD__
#define __DOT_MAINPROXY_H_HEADER_GUARD__

#include <stdint.h> // uint16_t

#include "PlatformContext.h"
#include "SPSCQueue.h"

namespace dot
{
namespace core
{

extern "C" int MainProxy(int argv, char ** argc, PlatformContext *platformContext);

struct WindowHandle
{
	WindowHandle(uint16_t idx) : mIdx(idx) {}

	uint16_t mIdx;
};
inline bool IsValid(WindowHandle handle) { return UINT16_MAX != handle.mIdx; }

class Event
{
public:
	enum Type
	{
		Exit,
		Size,
		Count
	};

	Event(Type type) : mType(type), mHandle(UINT16_MAX) {}

	Event(Type type, WindowHandle handle) : mType(type), mHandle(handle) {}

	Event::Type const GetType() const { return mType; }

protected:
	Event::Type mType;
	WindowHandle mHandle;
};

class SizeEvent : public Event
{
public:
	SizeEvent(WindowHandle handle) : Event(Event::Size, handle) {}

	uint32_t mWidth;
	uint32_t mHeight;
};

class EventQueue
{
public:
	EventQueue() {}
	
	~EventQueue()
	{
		for (const Event *evt = Poll(); nullptr != evt; evt = Poll())
		{
			Release(evt);
		}
	}

	void PostExitEvent()
	{
		Event *evt = new Event(Event::Exit);
		mQueue.enqueue(evt);
	}

	void PostSizeEvent(WindowHandle handle, uint32_t width, uint32_t height)
	{
		SizeEvent* evt = new SizeEvent(handle);
		evt->mWidth = width;
		evt->mHeight = height;
		mQueue.enqueue(evt);
	}

	const Event * Poll()
	{
		return mQueue.dequeue();
	}

	void Release(const Event * evt) const
	{
		if (evt != nullptr)
		{
			delete evt;
		}
	}

private:
	LockFreeQueue<Event> mQueue;
};

} // namespace core
} // namespace dot

#endif // __DOT_MAINPROXY_H_HEADER_GUARD__
#ifndef __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__
#define __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__

#include <stdint.h> // uint32_t
#include "core/StopWatch.h"

namespace dot { namespace rendering { class Renderer; } }

namespace dot
{
namespace core
{

class Application;
class EventQueue;

class PlatformContext : public StopWatchFactory
{
public:
	PlatformContext();
	virtual ~PlatformContext();

	virtual StopWatch * CreateStopWatch() = 0;

	virtual void PollEvents(Application * const app);
	virtual bool const IsExiting() const;

	dot::rendering::Renderer * const CreateRenderer();
	dot::rendering::Renderer * const GetRenderer() const { return mRenderer; }

	void * GetNativeWindowHandle() const { return mNativeWindowHandle; }

	const uint32_t GetFrameWidth() const { return mFrameWidth; }
	const uint32_t GetFrameHeight() const { return mFrameHeight; }

	const bool IsDebugModeEnabled() const { return mDebugMode; }

protected:
	EventQueue *mEventQueue;

	bool mExitRequest;
	bool mDebugMode;

	uint32_t mFrameWidth;
	uint32_t mFrameHeight;

	void* mNativeWindowHandle;

	dot::rendering::Renderer* mRenderer;
};

}
}

#endif // __DOT_PLATFORMCONTEXT_H_HEADER_GUARD__
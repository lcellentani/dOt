#ifndef __DOT_APPLICATION_H_HEADER_GUARD__
#define __DOT_APPLICATION_H_HEADER_GUARD__

#include <stdint.h> // uint32_t

namespace dot{ namespace core { class StopWatch; } }
namespace dot { namespace core { class PlatformContext; } }

namespace dot
{
namespace core
{

class Application
{

public:
	Application();
	virtual ~Application();

	virtual void Initialize(dot::core::PlatformContext * const platformContext);
	virtual void Update();
	virtual void Render() = 0;
	virtual int Shutdown() = 0;

	virtual void Reshape(uint32_t width, uint32_t height) {}

	const float GetTime() const { return mTime; }
	const float GetDeltaTime() const { return mDeltaTime; }

protected:
	StopWatch * mFrameTimer;
	float mTime;
	float mLastTime;
	float mDeltaTime;
};

extern "C" Application * CreateApplication();

} // namespace core
} // namespace dot

#endif // __DOT_APPLICATION_H_HEADER_GUARD__
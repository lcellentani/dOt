#include "core/Application.h"
#include "core/PlatformContext.h"
#include "core/StopWatch.h"

namespace dot
{
namespace core
{

Application::Application() : mFrameTimer(nullptr), mTime(0.0f), mLastTime(0.0f), mDeltaTime(0.0f)
{
}

Application::~Application()
{

}

void Application::Initialize(PlatformContext * const platformContext)
{
	mFrameTimer = platformContext->CreateStopWatch();
	mFrameTimer->Start();
}

void Application::Update()
{
	mLastTime = mTime;
	mTime = mFrameTimer->GetTime();
	mDeltaTime = mTime - mLastTime;
}

} // namespace core
} // namespace dot